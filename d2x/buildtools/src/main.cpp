// d2mcpp 的 d2x Provider。
//
//   describe        自我描述
//   exercises       枚举练习（有序）
//   check <id>      验证一道练习
//
// 全部输出为 NDJSON 事件流，一行一个事件。d2x 忽略解析不了的行，
// 所以 `mcpp run` 的前导空行之类的噪声不会破坏协议。
//
// 练习即测试：check 就是一次 `mcpp test -p <member> <name> --message-format
// json`，编译/退出码事实来自 mcpp 的 JSON 记录，运行期语义（断言/路障）
// 来自 harness 的侧信道 v2 —— 学习者绕过 d2x 直接 `mcpp test -p cpp11`
// 走的是完全相同的链路，不存在平行路径。
//
// 用法（由 .d2x.json 的 buildtools 字段驱动，从仓库根执行，无需 cd）：
//   mcpp run -q -p d2x/buildtools -- check cpp11-00-auto-and-decltype-0

import std;

import d2x.provider.emit;
import d2x.provider.discovery;
import d2x.provider.runner;

namespace fs = std::filesystem;

namespace {

constexpr int kProtocolVersion = 1;

// 向上找到仓库根：认 .d2x.json + mcpp.toml 同时存在。
// 不能依赖可执行文件位置——它在 target/<triple>/<hash>/bin/ 里，层数不固定。
std::optional<fs::path> find_repo_root() {
    auto dir = fs::current_path();
    for (int depth = 0; depth < 12; ++depth) {
        if (fs::exists(dir / ".d2x.json") && fs::exists(dir / "mcpp.toml"))
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
        d2x::emit::error(std::format("no exercises found under {} (lang={})",
                                     root.string(), lang));
        return 1;
    }

    for (const auto& ex : all) {
        std::vector<std::string> files{ex.file.string()};
        d2x::emit::exercise(ex.id, ex.order, ex.title, ex.chapter, files);
    }
    return 0;
}

// 侧信道里的 file 若是相对路径（编译器以工程目录为基准时），在协议边界
// 还原成绝对 —— d2x 靠它开编辑器、监听文件变更。展示归展示，定位归定位。
std::string to_abs(const fs::path& root, const std::string& member,
                   const std::string& file) {
    if (file.empty() || fs::path(file).is_absolute()) return file;
    return (root / member / file).lexically_normal().string();
}

int cmd_check(const fs::path& root, std::string_view id) {
    auto lang = d2x::discovery::read_lang(root);
    auto all  = d2x::discovery::scan(root, lang);

    auto it = std::ranges::find(all, id, &d2x::discovery::Exercise::id);
    if (it == all.end()) {
        d2x::emit::error(std::format("unknown exercise: {}", id));
        return 1;
    }

    // 侧信道放在 .d2x/ 下：学习者进度旁边，不污染工程目录。
    auto result_file = root / ".d2x" / "result.ndjson";

    // 从仓库根 spawn（Provider 由 d2x 在仓库根启动，mcpp 自己解析 workspace）
    std::error_code ec;
    fs::current_path(root, ec);

    d2x::emit::stage("compile");
    auto res = d2x::runner::run_mcpp_test(it->member, it->test_name, result_file);

    if (!res.package_error.empty()) {
        // 包级构建失败：harness 或工程本身坏了 —— 这是课程基础设施问题，
        // 不是这道题没做对。按协议约定不发 verdict（d2x 视为异常并原样呈现）。
        d2x::emit::output(res.package_error);
        d2x::emit::error("课程构建失败（不是这道练习的问题）——请向课程仓库反馈");
        return 1;
    }
    if (!res.record) {
        d2x::emit::error(std::format(
            "mcpp test 没有返回 '{}' 的记录{}", it->test_name,
            res.saw_any ? "" : "（mcpp 可能不支持 --message-format json，需要 >= 0.0.104）"));
        return 1;
    }

    const auto& rec = *res.record;
    d2x::emit::output(rec.compile_output);
    if (rec.status == "compile_fail") {
        d2x::emit::verdict("fail", "compile", rec.exit_code);
        return 0;   // 练习没通过是正常业务路径，不是 Provider 出错
    }

    d2x::emit::stage("run");
    d2x::emit::output(rec.run_output);

    auto report = d2x::runner::judge_run(rec.exit_code, result_file);

    std::vector<std::string> diags;
    for (const auto& f : report.failures) {
        diags.push_back(d2x::emit::diagnostic(to_abs(root, it->member, f.file),
                                              f.line, f.what, f.expected, f.actual));
    }

    d2x::emit::verdict(d2x::runner::to_string(report.outcome), "run",
                       rec.exit_code, diags);
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
        d2x::emit::error("repo root not found (looking for .d2x.json + mcpp.toml)");
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
