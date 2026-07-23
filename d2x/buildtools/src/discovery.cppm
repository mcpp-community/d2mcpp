// 练习发现：目录约定，别无其他。
//
// 刻意不引入独立的声明文件（exercises.toml 之类）。rustlings 最贵的一课
// 是 PR #1355：edition 同时写在 rustc 参数和 rust-project.json 里，两边漂移
// 酿成 bug。任何独立声明文件都是第二套真相源。这里的真相只有一处，且
// 无法漂移：目录结构本身。
//
//   src/intro/tests/hello-mcpp.cpp             → id: hello-mcpp   (chapter: intro)
//   src/cpp11/tests/00-auto-and-decltype/0.cpp → id: cpp11-00-auto-and-decltype-0
//   src/en/cpp11/tests/...                     lang=en 时启用，与 zh 互斥
//
// 每个 <std>/ 是真实 mcpp 工程，练习就是它的 tests/ —— 学习者可以绕过 d2x
// 直接 `mcpp test -p cpp11` 看进度表，Provider 走的是同一条路。
module;

#include <cstdio>   // stderr

export module d2x.provider.discovery;

import std;

namespace fs = std::filesystem;

namespace d2x::discovery {

export struct Exercise {
    std::string id;         // cpp11-00-auto-and-decltype-0（与旧布局完全一致，进度不丢）
    int         order{};    // 显式顺序，不依赖字典序
    std::string title;      // auto and decltype (0)
    std::string chapter;    // cpp11/00-auto-and-decltype
    std::string member;     // workspace 成员：cpp11 / intro / en/cpp11 …
    std::string test_name;  // mcpp test 里的名字：00-auto-and-decltype/0 / hello-mcpp
    fs::path    file;       // 绝对路径
};

// .d2x.json 中此处只需读取 "lang"。引入完整 JSON 解析器并无必要，
// 但也不能假装解析——这里只做一件明确的事：取出 "lang" 的字符串值。
export std::string read_lang(const fs::path& root) {
    std::ifstream in(root / ".d2x.json");
    if (!in) return "en";
    std::string text((std::istreambuf_iterator<char>(in)), {});

    auto key = text.find("\"lang\"");
    if (key == std::string::npos) return "en";
    auto colon = text.find(':', key);
    if (colon == std::string::npos) return "en";
    auto open = text.find('"', colon);
    if (open == std::string::npos) return "en";
    auto close = text.find('"', open + 1);
    if (close == std::string::npos) return "en";
    return text.substr(open + 1, close - open - 1);
}

// cpp11 -> 11。用于排序，让 cpp11 的练习永远排在 cpp14 之前。
int std_rank(std::string_view dir) {
    if (dir.starts_with("cpp")) {
        int n = 0;
        auto rest = dir.substr(3);
        auto [_, ec] = std::from_chars(rest.data(), rest.data() + rest.size(), n);
        if (ec == std::errc{}) return n;
    }
    return -1;  // intro
}

// 00-auto-and-decltype -> { chapter_no=0, topic="auto-and-decltype" }
struct ChapterParts { int chapter_no; std::string topic; };

std::optional<ChapterParts> parse_chapter(std::string_view dir_name) {
    auto dash = dir_name.find('-');
    if (dash == std::string_view::npos) return std::nullopt;

    int chapter_no = 0;
    auto head = dir_name.substr(0, dash);
    if (std::from_chars(head.data(), head.data() + head.size(), chapter_no).ec != std::errc{})
        return std::nullopt;
    return ChapterParts{chapter_no, std::string(dir_name.substr(dash + 1))};
}

std::string humanize(std::string_view topic) {
    std::string out(topic);
    std::ranges::replace(out, '-', ' ');
    return out;
}

// 练习 id 直接来自目录/文件名，而它有两个危险去向：
//   1. d2x 把它拼进一条 shell 命令（`<provider> check <id>`）
//   2. 我们把 test 名拼进 `mcpp test <pattern>` 命令行
// 带反引号、引号或换行的名字在任一处都可能越界。在发现阶段拒绝比在下游各自转义更可靠。
export bool valid_id(std::string_view id) {
    if (id.empty()) return false;
    return std::ranges::all_of(id, [](unsigned char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.';
    });
}

// test 名多允许一个 '/'（章节目录分隔符），其余同 id 白名单。
export bool valid_test_name(std::string_view name) {
    if (name.empty()) return false;
    return std::ranges::all_of(name, [](unsigned char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.'
            || c == '/';
    });
}

// 扫描一个成员工程的 tests/。std_dir 是去掉 en/ 前缀后的标准名（intro 为空串）。
void scan_member(const fs::path& repo_root, const std::string& member,
                 const std::string& std_dir, std::vector<Exercise>& out) {
    auto tests = repo_root / member / "tests";
    if (!fs::is_directory(tests)) return;

    for (const auto& entry : fs::recursive_directory_iterator(tests)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".cpp") continue;

        auto rel = fs::relative(entry.path(), tests);
        Exercise ex;
        ex.file      = fs::absolute(entry.path()).lexically_normal();
        ex.member    = member;
        ex.test_name = rel.parent_path().empty()
                     ? rel.stem().string()
                     : std::format("{}/{}", rel.parent_path().generic_string(),
                                   rel.stem().string());

        auto chapter_dir = rel.parent_path().filename().string();
        auto parts = chapter_dir.empty() ? std::nullopt : parse_chapter(chapter_dir);
        if (parts) {
            int index = 0;
            auto stem = rel.stem().string();
            std::from_chars(stem.data(), stem.data() + stem.size(), index);
            ex.id      = std::format("{}-{}-{}", std_dir, chapter_dir, stem);
            ex.chapter = std::format("{}/{}", std_dir, chapter_dir);
            ex.title   = std::format("{} ({})", humanize(parts->topic), index);
            ex.order   = std_rank(std_dir) * 100'000 + parts->chapter_no * 100 + index;
        } else {
            // intro 之类不带章节目录的练习（tests/hello-mcpp.cpp）
            ex.id      = rel.stem().string();
            ex.chapter = "intro";
            ex.title   = humanize(rel.stem().string());
            ex.order   = -100'000;
        }

        // 拒绝而不是转义：这类名字是课程作者的笔误或恶意 PR，
        // 静默接受只会把问题推到下游。
        if (!valid_id(ex.id) || !valid_test_name(ex.test_name)) {
            std::println(stderr,
                "d2x-buildtools-mcpp: 跳过 '{}' —— 练习 id/名字 只允许 [A-Za-z0-9._/-]",
                ex.file.string());
            continue;
        }
        out.push_back(std::move(ex));
    }
}

// 扫描全部练习。课程工程住在 src/ 下；lang=en 用 src/en/ 镜像。
export std::vector<Exercise> scan(const fs::path& repo_root, std::string_view lang) {
    std::string prefix = (lang == "en") ? "src/en/" : "src/";
    std::vector<Exercise> found;

    scan_member(repo_root, prefix + "intro", "", found);

    std::vector<std::string> std_dirs;
    auto base = (lang == "en") ? repo_root / "src" / "en" : repo_root / "src";
    if (fs::is_directory(base)) {
        for (const auto& entry : fs::directory_iterator(base)) {
            auto name = entry.path().filename().string();
            if (entry.is_directory() && name.starts_with("cpp")
                && fs::is_directory(entry.path() / "tests"))
                std_dirs.push_back(name);
        }
    }
    std::ranges::sort(std_dirs);
    for (const auto& d : std_dirs) scan_member(repo_root, prefix + d, d, found);

    std::ranges::sort(found, {}, &Exercise::order);
    return found;
}

} // namespace d2x::discovery
