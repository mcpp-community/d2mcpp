// 练习发现：目录约定 + 练习文件内的就近指令。
//
// 刻意不引入独立的声明文件（exercises.toml 之类）。rustlings 最贵的一课
// 是 PR #1355：edition 同时写在 rustc 参数和 rust-project.json 里，两边漂移
// 酿成 bug。任何独立声明文件都是第二套真相源。这里的真相只有两处，且都
// 无法漂移：目录结构，和练习文件自己的头部注释。
module;

#include <cstdio>   // stderr

export module d2x.provider.discovery;

import std;

namespace fs = std::filesystem;

namespace d2x::discovery {

export struct Exercise {
    std::string id;        // cpp11-00-auto-and-decltype-0
    int         order{};   // 显式顺序，不依赖字典序
    std::string title;     // auto and decltype (0)
    std::string chapter;   // cpp11/00-auto-and-decltype
    std::string std_dir;   // cpp11          —— 决定它属于哪个 member
    fs::path    file;      // 绝对路径
    std::vector<std::string> cxxflags;  // 来自 // d2x:cxxflags: 指令
};

// .d2x.json 里我们只关心 "lang"。写一个完整 JSON 解析器是杀鸡用牛刀，
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
    return -1;  // 根目录下的 hello-mcpp
}

// 从练习文件头部读 `// d2x:cxxflags: -O0 -fno-elide-constructors`。
// 只扫前 40 行——指令属于文件头的元信息区，不该藏在代码中间。
std::vector<std::string> read_cxxflags(const fs::path& file) {
    std::ifstream in(file);
    if (!in) return {};

    constexpr std::string_view kMarker = "d2x:cxxflags:";
    std::string line;
    for (int n = 0; n < 40 && std::getline(in, line); ++n) {
        auto pos = line.find(kMarker);
        if (pos == std::string::npos) continue;

        std::vector<std::string> flags;
        std::istringstream rest(line.substr(pos + kMarker.size()));
        for (std::string flag; rest >> flag; ) flags.push_back(flag);
        return flags;
    }
    return {};
}

// 00-auto-and-decltype-0 -> { chapter_no=0, topic="auto-and-decltype", index=0 }
// 03-trailing-return-type -> { chapter_no=3, topic="trailing-return-type", index=0 }
//
// 尾部的 -<数字> 是练习序号；没有就当 0。注意 d2mcpp 现有命名里
// 03/04 两章确实缺了 -0 后缀，这个规则把它们归一化掉。
struct NameParts { int chapter_no; std::string topic; int index; };

std::optional<NameParts> parse_stem(std::string_view stem) {
    auto dash = stem.find('-');
    if (dash == std::string_view::npos) return std::nullopt;

    int chapter_no = 0;
    auto head = stem.substr(0, dash);
    if (std::from_chars(head.data(), head.data() + head.size(), chapter_no).ec != std::errc{})
        return std::nullopt;

    auto rest = stem.substr(dash + 1);
    int index = 0;
    auto last = rest.rfind('-');
    if (last != std::string_view::npos) {
        auto tail = rest.substr(last + 1);
        int parsed = 0;
        auto [_, ec] = std::from_chars(tail.data(), tail.data() + tail.size(), parsed);
        if (ec == std::errc{} && !tail.empty()) {
            index = parsed;
            rest  = rest.substr(0, last);
        }
    }
    return NameParts{chapter_no, std::string(rest), index};
}

std::string humanize(std::string_view topic) {
    std::string out(topic);
    std::ranges::replace(out, '-', ' ');
    return out;
}

// 练习 id 直接来自文件名，而它有两个危险去向：
//   1. d2x 把它拼进一条 shell 命令（`<provider> check <id>`）
//   2. 我们把它写进生成的 TOML（`[targets.<id>]`）
// 带反引号、`]`、引号或换行的文件名能在任一处越界。
//
// 在源头挡住比在两个下游各自转义更可靠 —— 课程里本就不该出现这种文件名，
// 与其想办法安全地传递它，不如明确拒绝并让作者改名。
export bool valid_id(std::string_view id) {
    if (id.empty()) return false;
    return std::ranges::all_of(id, [](unsigned char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.';
    });
}

// 扫描练习根目录。lang=zh 用 dslings/，lang=en 用 dslings/en/。
export std::vector<Exercise> scan(const fs::path& repo_root, std::string_view lang) {
    fs::path base = repo_root / "dslings";
    if (lang == "en") base /= "en";
    if (!fs::is_directory(base)) return {};

    std::vector<Exercise> found;

    auto add = [&](const fs::path& file, const std::string& std_dir) {
        auto stem  = file.stem().string();
        auto parts = parse_stem(stem);

        Exercise ex;
        ex.file     = fs::absolute(file).lexically_normal();
        ex.std_dir  = std_dir;
        ex.cxxflags = read_cxxflags(file);

        if (parts) {
            ex.id      = std_dir.empty() ? stem : std::format("{}-{}", std_dir, stem);
            ex.chapter = std_dir.empty()
                       ? std::format("{:02}-{}", parts->chapter_no, parts->topic)
                       : std::format("{}/{:02}-{}", std_dir, parts->chapter_no, parts->topic);
            ex.title   = std::format("{} ({})", humanize(parts->topic), parts->index);
            ex.order   = std_rank(std_dir) * 100'000 + parts->chapter_no * 100 + parts->index;
        } else {
            // 不符合 NN-topic 约定的（如根目录的 hello-mcpp.cpp）
            ex.id      = std_dir.empty() ? stem : std::format("{}-{}", std_dir, stem);
            ex.chapter = std_dir.empty() ? "intro" : std_dir;
            ex.title   = humanize(stem);
            ex.order   = std_rank(std_dir) * 100'000;
        }

        // 拒绝而不是转义：这类文件名是课程作者的笔误或恶意 PR，
        // 静默接受只会把问题推到下游。
        if (!valid_id(ex.id)) {
            std::println(stderr,
                "d2x-buildtools-mcpp: 跳过 '{}' —— 练习 id 只允许 [A-Za-z0-9._-]",
                ex.file.string());
            return;
        }
        found.push_back(std::move(ex));
    };

    // 根目录下的入门练习（hello-mcpp.cpp）
    for (const auto& entry : fs::directory_iterator(base)) {
        if (entry.is_regular_file() && entry.path().extension() == ".cpp") add(entry.path(), "");
    }

    // 按标准分的子目录
    std::vector<fs::path> std_dirs;
    for (const auto& entry : fs::directory_iterator(base)) {
        if (entry.is_directory() && entry.path().filename().string().starts_with("cpp"))
            std_dirs.push_back(entry.path());
    }
    std::ranges::sort(std_dirs);

    for (const auto& dir : std_dirs) {
        auto std_dir = dir.filename().string();
        std::vector<fs::path> files;
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".cpp")
                files.push_back(entry.path());
        }
        std::ranges::sort(files);
        for (const auto& f : files) add(f, std_dir);
    }

    std::ranges::sort(found, {}, &Exercise::order);
    return found;
}

} // namespace d2x::discovery
