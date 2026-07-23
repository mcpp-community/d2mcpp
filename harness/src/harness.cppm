module;

// C 运行时符号走全局模块片段：atexit/_Exit/getenv/fopen 一族 import std 不提供。
#include <cstdio>
#include <cstdlib>

export module d2x.harness;

import std;

// d2mcpp 练习脚手架 —— 唯一路径，纯模块，零宏。
//
// 填空占位符 D2X_YOUR_ANSWER 不在这里：它是**纯约定**，不定义在任何地方。
// 一个未定义的标识符本身就是编译错误，而且报错正好指着要填的位置
// （`unknown type name 'D2X_YOUR_ANSWER'`）——比旧宏展开为空后指着别处的
// 级联报错更好。所以它不需要头文件、在模块化练习里天然可用、拷进
// Compiler Explorer 也成立。
//
// 判定信号走两条路，各有消费者：
//   进程退出码   —— 有失败断言或未拆的 wait() 时退出码变 1。这让裸
//                   `mcpp test` 不需要懂任何 d2x 概念就能显示对错。
//   侧信道 v2    —— D2X_RESULT_FILE 指定的 NDJSON 文件，Provider 读它
//                   把「为什么失败」变成结构化诊断、把 wait 区分成 blocked。
//                   未设置时不写文件：学员直接跑二进制零摩擦。

namespace d2x::detail {

int  g_failures = 0;
int  g_waits    = 0;
bool g_hooked   = false;

// 进程正常退出时,若有失败断言或未拆的路障,把退出码改成 1。
// 只在这两种情况下覆盖——练习自己 return 非 0 时不动它。
// _Exit 跳过后续清理,所以先冲刷 stdio。
void exit_hook() {
    if (d2x::detail::g_failures > 0 || d2x::detail::g_waits > 0) {
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
    if (!path || !*path) return;                 // 学员直接运行：什么都不做

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

inline std::string show_impl(const auto& v) {
    if constexpr (requires { std::format("{}", v); }) return std::format("{}", v);
    else return {};
}

} // namespace d2x::detail

export namespace d2x {

// 可见输出格式沿用旧宏时代的样子（[HONLY LOGI]: - ✅ | ...）——book 里
// 引用了这些输出，格式漂移就是文档漂移。`what` 是断言的语义标签（通常是
// 表达式原文，由撰稿或迁移脚本填写），补上 c++23 无反射拿不到 #expr 的缺。

inline bool check(bool ok, std::string_view what = {},
                  std::source_location loc = std::source_location::current()) {
    d2x::detail::ensure_hook();
    if (!ok) ++d2x::detail::g_failures;

    auto label = what.empty() ? std::string_view("check") : what;
    d2x::detail::report_assert(ok, label, "true", ok ? "true" : "false",
                               loc.file_name(), static_cast<int>(loc.line()));
    if (ok) std::print("\033[32m[HONLY LOGI]: - ✅ | {}\033[0m\n", label);
    else    std::print("\033[33m[HONLY LOGW]: {}:{} - ❌(error) | {}\033[0m\n",
                       loc.file_name(), loc.line(), label);
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

    if (ok) std::print("\033[32m[HONLY LOGI]: - ✅ | {} ({} == {})\033[0m\n", label, sa, sb);
    else    std::print("\033[33m[HONLY LOGW]: {}:{} - ❌ | {} ({} == {})\033[0m\n",
                       loc.file_name(), loc.line(), label, sa, sb);
    std::fflush(stdout);
    return ok;
}

// 显式路障：学员读完说明、删掉这一行才算真正完成这一题。
// 记录后照常返回（不中断程序）——后续断言还要跑；退出码由 exit_hook 收口，
// Provider 再根据侧信道把「只剩 wait」区分成 blocked。
inline void wait(std::source_location loc = std::source_location::current()) {
    d2x::detail::ensure_hook();
    ++d2x::detail::g_waits;
    d2x::detail::report_wait(loc.file_name(), static_cast<int>(loc.line()));
    std::print("\033[33m[HONLY LOGW]: {}:{} - 🥳 Delete the d2x::wait() to continue...\033[0m\n",
               loc.file_name(), loc.line());
    std::fflush(stdout);
}

} // namespace d2x
