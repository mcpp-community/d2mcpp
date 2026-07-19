// 生成 mcpp workspace 清单。
//
// 双 member 布局，解决一个硬约束：dslings 的练习默认就编译不过，而 mcpp 的
// build/run 都会先全量构建整个包——一个坏兄弟拖垮全部且零产物。所以：
//
//   .d2x/build/<std>/     全量 target，供 clangd 拿到完整 compile_commands.json
//   .d2x/build/_current/  每次只写当前一题，checker 只构建它
//
// 生成物全部在 .d2x/build/ 下，练习源文件原地不动（main 用 ../ 逃逸出包根）。
export module d2x.provider.manifest;

import std;
import d2x.provider.discovery;

namespace fs = std::filesystem;

namespace d2x::manifest {

using discovery::Exercise;

// 所有练习统一按 c++23 编译。mcpp 目前硬拒 c++11/14/17/20
// （src/manifest/types.cppm 的白名单），本方案选择不改 mcpp 上游。
constexpr std::string_view kStandard = "c++23";

// 生成的 member 位于 .d2x/build/<name>/，回到仓库根要退三层。
constexpr std::string_view kToRoot = "../../..";

export fs::path build_dir(const fs::path& repo_root) {
    return repo_root / ".d2x" / "build";
}

std::string member_name(const Exercise& ex) {
    return ex.std_dir.empty() ? "intro" : ex.std_dir;
}

// 把绝对路径转成相对 member 目录的形式。mcpp 的 main 接受 ../ 逃逸。
std::string relative_to_member(const fs::path& repo_root, const fs::path& file) {
    auto rel = fs::relative(file, repo_root).generic_string();
    return std::format("{}/{}", kToRoot, rel);
}

void write_package_header(std::ostream& out, std::string_view name) {
    std::println(out, "# 由 d2x-buildtools-mcpp 生成，请勿手工编辑。");
    std::println(out, "[package]");
    std::println(out, "name     = \"{}\"", name);
    std::println(out, "version  = \"0.1.0\"");
    std::println(out, "standard = \"{}\"", kStandard);
    std::println(out, "");
    std::println(out, "[build]");
    // 显式清空源码 glob：练习不是这个包的「源码」，它们只是各自 target 的入口。
    std::println(out, "sources      = []");
    std::println(out, "");
    // 脚手架走正经的库依赖，而不是把仓库根塞进 include 搜索路径。
    // 后者会让练习能 #include 仓库里任何文件，是个隐患。
    std::println(out, "[dependencies]");
    std::println(out, "harness = {{ path = \"{}/dslings/harness\" }}", kToRoot);
}

void write_target(std::ostream& out, const fs::path& repo_root, const Exercise& ex) {
    std::println(out, "");
    std::println(out, "[targets.{}]", ex.id);
    std::println(out, "kind = \"bin\"");
    std::println(out, "main = \"{}\"", relative_to_member(repo_root, ex.file));
    if (!ex.cxxflags.empty()) {
        std::string list;
        for (std::size_t i = 0; i < ex.cxxflags.size(); ++i) {
            if (i) list += ", ";
            list += std::format("\"{}\"", ex.cxxflags[i]);
        }
        std::println(out, "cxxflags = [{}]", list);
    }
}

// 只有内容真的变了才落盘。避免无谓地推进 mtime，让 mcpp 的快速路径失效。
bool write_if_changed(const fs::path& path, std::string_view content) {
    if (fs::exists(path)) {
        std::ifstream in(path);
        std::string existing((std::istreambuf_iterator<char>(in)), {});
        if (existing == content) return false;
    }
    fs::create_directories(path.parent_path());
    std::ofstream out(path);
    out << content;
    return true;
}

// 全量清单：每个 C++ 标准一个 member，持有该标准下的全部练习。
// 供 clangd —— 即使大多数练习编译不过，mcpp 仍会生成完整的
// compile_commands.json（已实测），所以 IDE 对每个练习都有正确的编译参数。
export void write_full(const fs::path& repo_root, const std::vector<Exercise>& all) {
    auto root = build_dir(repo_root);

    std::map<std::string, std::vector<const Exercise*>> by_member;
    for (const auto& ex : all) by_member[member_name(ex)].push_back(&ex);

    std::vector<std::string> members;
    for (const auto& [name, list] : by_member) {
        std::ostringstream buf;
        write_package_header(buf, name);
        for (const auto* ex : list) write_target(buf, repo_root, *ex);
        write_if_changed(root / name / "mcpp.toml", buf.str());
        members.push_back(name);
    }

    // _current 也是这个 workspace 的成员，占位清单先写空壳
    members.push_back("_current");
    write_if_changed(root / "_current" / "mcpp.toml",
                     [&] { std::ostringstream b; write_package_header(b, "_current"); return b.str(); }());

    std::ostringstream ws;
    std::println(ws, "# 由 d2x-buildtools-mcpp 生成，请勿手工编辑。");
    std::println(ws, "[workspace]");
    std::print(ws, "members = [");
    for (std::size_t i = 0; i < members.size(); ++i) {
        if (i) std::print(ws, ", ");
        std::print(ws, "\"{}\"", members[i]);
    }
    std::println(ws, "]");
    write_if_changed(root / "mcpp.toml", ws.str());
}

// 单题清单：checker 每次 check 前重写它，只含要验证的那一题。
// 实测改写 target 集合不会让 fingerprint 目录爆炸（始终只有 1 个），
// 切题 0.118s、切回 0.018s。
export void write_current(const fs::path& repo_root, const Exercise& ex) {
    std::ostringstream buf;
    write_package_header(buf, "_current");
    write_target(buf, repo_root, ex);
    write_if_changed(build_dir(repo_root) / "_current" / "mcpp.toml", buf.str());
}

} // namespace d2x::manifest
