module;

// C 运行时符号走全局模块片段：atexit/_Exit/getenv/fopen 一族 import std 不提供。
#include <cstdio>
#include <cstdlib>

export module d2x;

import std;

// d2x 练习库 —— 唯一路径，纯模块，零宏。练习侧 `import d2x;` 即可。
//
// 填空占位符 D2X_YOUR_ANSWER 不在这里：它是**纯约定**，不定义在任何地方。
// 一个未定义的标识符本身就是编译错误，而且报错正好指着要填的位置
// （`unknown type name 'D2X_YOUR_ANSWER'`）。所以它不需要任何导入、在
// 模块化练习里天然可用、拷进 Compiler Explorer 也成立。
//
// 判定信号走两条路，各有消费者：
//   进程退出码   —— 有失败检查点或未拆的 wait() 时退出码变 1。这让裸
//                   `mcpp test` 不需要懂任何 d2x 概念就能显示对错。
//   侧信道 v2    —— D2X_RESULT_FILE 指定的 NDJSON 文件，Provider 读它
//                   把「为什么失败」变成结构化诊断、把 wait 区分成 blocked。
//                   未设置时不写文件：学习者直接跑二进制零摩擦。
//
// 可见输出的设计（学习者直接看到的三类输出行）：
//   ✅ | <what> (<actual> == <expected>)                     绿
//   ❌ | <what> (<actual> == <expected>)  -> <file>:<line>  红
//   🚧 | Delete the d2x::wait() to continue  -> <file>:<line>  黄
// 标识就是 emoji 本身，不带日志框架前缀；file 尽量以仓库相对路径展示
// （定位用的绝对路径走侧信道，展示归展示、定位归定位）。

namespace d2x::detail {

int  g_failures = 0;
int  g_waits    = 0;
bool g_hooked   = false;

// 进程正常退出时,若有失败检查点或未拆的路障,把退出码改成 1。
// 只在这两种情况下覆盖——练习自己 return 非 0 时不动它。
// _Exit 跳过后续清理,所以先冲刷 stdio。
void exit_hook() {
    if (g_failures > 0 || g_waits > 0) {
        std::fflush(stdout);
        std::fflush(stderr);
        std::_Exit(1);
    }
}

void ensure_hook() {
    if (!g_hooked) {
        g_hooked = true;
        std::atexit(exit_hook);
    }
}

inline std::string escape(std::string_view s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (unsigned char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:
                if (c < 0x20) out += std::format("\\u{:04x}", static_cast<int>(c));
                else          out += static_cast<char>(c);
        }
    }
    return out;
}

// 每次都重新打开并追加。比持有 FILE* 慢，但换来两个性质：
// 进程异常终止不丢已写入的行；无需依赖静态析构顺序。
inline void emit(const std::string& json_line) {
    const char* path = std::getenv("D2X_RESULT_FILE");
    if (!path || !*path) return;                 // 学习者直接运行：什么都不做

    if (std::FILE* f = std::fopen(path, "a")) {
        std::fputs(json_line.c_str(), f);
        std::fputc('\n', f);
        std::fclose(f);
    }
}

inline void report_assert(bool ok, std::string_view what,
                          const std::string& expected, const std::string& actual,
                          const char* file, int line) {
    std::string out = "{\"v\":2,\"kind\":\"assert\",\"ok\":";
    out += ok ? "true" : "false";
    out += ",\"what\":\""     + escape(what)     + "\"";
    out += ",\"expected\":\"" + escape(expected) + "\"";
    out += ",\"actual\":\""   + escape(actual)   + "\"";
    out += ",\"file\":\""     + escape(file ? file : "") + "\"";
    out += ",\"line\":" + std::to_string(line) + "}";
    emit(out);
}

inline void report_wait(const char* file, int line) {
    std::string out = "{\"v\":2,\"kind\":\"wait\",\"file\":\"";
    out += escape(file ? file : "");
    out += "\",\"line\":" + std::to_string(line) + "}";
    emit(out);
}

// std::formattable 是 SFINAE 安全的探测；若直接在 requires 表达式里写
// std::format("{}", v)，会触发未特化 std::formatter 的 static_assert
// 硬错误（scoped enum、自定义类型的练习中已实际出现）。
inline std::string show_impl(const auto& v) {
    using T = std::remove_cvref_t<decltype(v)>;
    if constexpr (std::formattable<T, char>) return std::format("{}", v);
    else return {};
}

// 展示用路径：能剥掉当前工作目录前缀就剥（学习者从仓库根跑时看到
// src/cpp11/tests/... 而不是一长串 /home/...）。剥不掉就原样。
inline std::string show_path(const char* file) {
    if (!file) return {};
    std::string_view path(file);
    std::error_code ec;
    auto cwd = std::filesystem::current_path(ec);
    if (!ec) {
        auto prefix = cwd.generic_string() + "/";
        if (path.starts_with(prefix)) return std::string(path.substr(prefix.size()));
    }
    return std::string(path);
}

constexpr std::string_view kGreen  = "\033[32m";
constexpr std::string_view kRed    = "\033[31m";
constexpr std::string_view kYellow = "\033[33m";
constexpr std::string_view kReset  = "\033[0m";

} // namespace d2x::detail

export namespace d2x {

// 检查点。`what` 是给学习者看的语义标签（通常是表达式原文——c++23 没有
// 反射拿不到 #expr，由撰稿或迁移脚本填写）。
inline bool check(bool ok, std::string_view what = {},
                  std::source_location loc = std::source_location::current()) {
    d2x::detail::ensure_hook();
    if (!ok) ++d2x::detail::g_failures;

    auto label = what.empty() ? std::string_view("check") : what;
    d2x::detail::report_assert(ok, label, "true", ok ? "true" : "false",
                               loc.file_name(), static_cast<int>(loc.line()));
    if (ok) std::print("{}✅ | {}{}\n", d2x::detail::kGreen, label, d2x::detail::kReset);
    else    std::print("{}❌ | {}  -> {}:{}{}\n", d2x::detail::kRed, label,
                       d2x::detail::show_path(loc.file_name()), loc.line(),
                       d2x::detail::kReset);
    std::fflush(stdout);
    return ok;
}

template<typename A, typename B>
inline bool check_eq(const A& a, const B& b, std::string_view what = {},
                     std::source_location loc = std::source_location::current()) {
    d2x::detail::ensure_hook();
    const bool ok = (a == b);
    if (!ok) ++d2x::detail::g_failures;

    auto sa = d2x::detail::show_impl(a);
    auto sb = d2x::detail::show_impl(b);
    auto label = what.empty() ? std::format("{} == {}", sa, sb) : std::string(what);

    d2x::detail::report_assert(ok, label, sb, sa,
                               loc.file_name(), static_cast<int>(loc.line()));

    if (ok) std::print("{}✅ | {} ({} == {}){}\n",
                       d2x::detail::kGreen, label, sa, sb, d2x::detail::kReset);
    else    std::print("{}❌ | {} ({} == {})  -> {}:{}{}\n",
                       d2x::detail::kRed, label, sa, sb,
                       d2x::detail::show_path(loc.file_name()), loc.line(),
                       d2x::detail::kReset);
    std::fflush(stdout);
    return ok;
}

// 恒等透传：标记「这一行是教学观测点，别删」。
template<typename T>
constexpr decltype(auto) dont_delete_this(T&& x) {
    return std::forward<T>(x);
}

// 显式路障：学习者读完说明、删掉这一行才算真正完成这一题。
// 记录后照常返回（不中断程序）——后续检查点还要跑；退出码由 exit_hook
// 收口，Provider 再根据侧信道把「只剩 wait」区分成 blocked。
inline void wait(std::source_location loc = std::source_location::current()) {
    d2x::detail::ensure_hook();
    ++d2x::detail::g_waits;
    d2x::detail::report_wait(loc.file_name(), static_cast<int>(loc.line()));
    std::print("{}🚧 | Delete the d2x::wait() to continue  -> {}:{}{}\n",
               d2x::detail::kYellow, d2x::detail::show_path(loc.file_name()),
               loc.line(), d2x::detail::kReset);
    std::fflush(stdout);
}

} // namespace d2x
