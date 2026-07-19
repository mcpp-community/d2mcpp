// d2mcpp 的 d2x Provider。
//
//   describe        自我描述
//   exercises       枚举练习（有序）
//   check <id>      验证一道练习
//
// 全部输出为 NDJSON 事件流，一行一个事件。d2x 忽略解析不了的行，
// 所以 `mcpp run` 的前导空行之类的噪声不会破坏协议。
//
// 用法（由 .d2x.json 的 buildtools 字段驱动，从仓库根执行，无需 cd）：
//   mcpp run -q -p d2x/buildtools/mcpp -- check cpp11-00-auto-and-decltype-0

import std;

import d2x.provider.emit;
import d2x.provider.discovery;
import d2x.provider.manifest;
import d2x.provider.runner;

namespace fs = std::filesystem;

namespace {

constexpr int kProtocolVersion = 1;

// 向上找到仓库根：认 .d2x.json + dslings/ 同时存在。
// 不能依赖可执行文件位置——它在 target/<triple>/<hash>/bin/ 里，层数不固定。
std::optional<fs::path> find_repo_root() {
    auto dir = fs::current_path();
    for (int depth = 0; depth < 12; ++depth) {
        if (fs::exists(dir / ".d2x.json") && fs::is_directory(dir / "dslings"))
            return dir;
        if (!dir.has_parent_path() || dir.parent_path() == dir) break;
        dir = dir.parent_path();
    }
    return std::nullopt;
}

int cmd_describe() {
    d2x::emit::describe("mcpp", kProtocolVersion);
    return 0;
}

int cmd_exercises(const fs::path& root) {
    auto lang = d2x::discovery::read_lang(root);
    auto all  = d2x::discovery::scan(root, lang);
    if (all.empty()) {
        d2x::emit::error(std::format("no exercises found under {}/dslings (lang={})",
                                     root.string(), lang));
        return 1;
    }

    // 顺手刷新全量清单：clangd 依赖它拿到每个练习的编译参数。
    // 这里只写文件不构建——构建是 check 的事，枚举必须快。
    d2x::manifest::write_full(root, all);

    for (const auto& ex : all) {
        std::vector<std::string> files{ex.file.string()};
        d2x::emit::exercise(ex.id, ex.order, ex.title, ex.chapter, files);
    }
    return 0;
}

int cmd_check(const fs::path& root, std::string_view id) {
    auto lang = d2x::discovery::read_lang(root);
    auto all  = d2x::discovery::scan(root, lang);

    auto it = std::ranges::find(all, id, &d2x::discovery::Exercise::id);
    if (it == all.end()) {
        d2x::emit::error(std::format("unknown exercise: {}", id));
        return 1;
    }

    // 先保证整个 workspace 存在。check 可能是全新仓库上的第一条命令
    // （学员直接跑 d2x checker），此时根清单还没生成，mcpp 会以退出码 2
    // 报 "workspace member not found"。write_* 都是内容比对后才落盘，
    // 重复调用不会推进 mtime、不会让 mcpp 的快速路径失效。
    d2x::manifest::write_full(root, all);

    // 再把这一题单独写进 _current，其余练习（多半还编译不过）不参与构建
    d2x::manifest::write_current(root, *it);
    d2x::runner::enter(d2x::manifest::build_dir(root));

    d2x::emit::stage("compile");
    auto compiled = d2x::runner::build_current();
    d2x::emit::output(compiled.output);
    if (compiled.exit_code != 0) {
        d2x::emit::verdict("fail", "compile", compiled.exit_code);
        return 0;   // 练习没通过是正常业务路径，不是 Provider 出错
    }

    d2x::emit::stage("run");

    // 侧信道放在构建目录里：与生成物同生共死，不污染仓库
    auto result_file = d2x::manifest::build_dir(root) / "_current" / "result.ndjson";
    auto ran = d2x::runner::run_current(result_file);
    d2x::emit::output(ran.output);

    auto report = d2x::runner::judge_run(ran.exit_code, result_file);

    std::vector<std::string> diags;
    for (const auto& f : report.failures) {
        diags.push_back(d2x::emit::diagnostic(f.file, f.line, f.expr, f.expected, f.actual));
    }

    d2x::emit::verdict(d2x::runner::to_string(report.outcome), "run", ran.exit_code, diags);
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    std::vector<std::string_view> args(argv + 1, argv + argc);

    if (args.empty()) {
        d2x::emit::error("usage: describe | exercises | check <id>");
        return 2;
    }

    if (args[0] == "describe") return cmd_describe();

    auto root = find_repo_root();
    if (!root) {
        d2x::emit::error("repo root not found (looking for .d2x.json + dslings/)");
        return 2;
    }

    if (args[0] == "exercises") return cmd_exercises(*root);

    if (args[0] == "check") {
        if (args.size() < 2) {
            d2x::emit::error("check requires an exercise id");
            return 2;
        }
        return cmd_check(*root, args[1]);
    }

    d2x::emit::error(std::format("unknown command: {}", args[0]));
    return 2;
}
