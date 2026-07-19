module;

// 上报核心以头文件形式共享给两条路径。放在全局模块片段里引入，
// 这样模块实现能用它，而它的宏（无）不会泄漏给消费者。
#include <d2x/cpp/report.hpp>
#include <cstdio>

export module d2x.harness;

import std;

// 模块化练习的脚手架 —— import 路径。
//
// 与 #include 路径的差异，写在这里免得踩坑：
//
//   宏无法跨模块导出。所以这里提供的是函数，不是 d2x_assert 那套宏。
//   好处是 std::source_location 自动带上 file/line，比 __LINE__ 更准，
//   而且正好是 Provider 填 Verdict.diagnostics 需要的东西；代价是丢了
//   表达式原文（宏版能打印 "a == b"，函数版只能打印值）。
//
//   D2X_YOUR_ANSWER 没有模块等价物 —— 它必须展开为空才能制造编译错误，
//   本质就是宏。模块化章节需要另一套填空约定。
export namespace d2x {

// 与 #include 路径共用同一套可见输出格式，学员在两种练习里看到的是一致的。
inline void check(bool ok, std::source_location loc = std::source_location::current()) {
    d2x::report::assertion(ok, "check", "true", ok ? "true" : "false",
                           loc.file_name(), static_cast<int>(loc.line()));
    if (ok) std::print("\033[32m[HONLY LOGI]: - ✅ | check\033[0m\n");
    else    std::print("\033[33m[HONLY LOGW]: {}:{} - ❌(error) | check\033[0m\n",
                       loc.file_name(), loc.line());
    std::fflush(stdout);
}

template<typename A, typename B>
inline void check_eq(const A& a, const B& b,
                     std::source_location loc = std::source_location::current()) {
    const bool ok = (a == b);

    auto show = [](const auto& v) -> std::string {
        if constexpr (requires { std::format("{}", v); }) return std::format("{}", v);
        else return {};
    };

    d2x::report::assertion(ok, "check_eq", show(b), show(a),
                           loc.file_name(), static_cast<int>(loc.line()));

    if (ok) std::print("\033[32m[HONLY LOGI]: - ✅ | {} == {}\033[0m\n", show(a), show(b));
    else    std::print("\033[33m[HONLY LOGW]: {}:{} - ❌ | {} == {}\033[0m\n",
                       loc.file_name(), loc.line(), show(a), show(b));
    std::fflush(stdout);
}

// D2X_WAIT 的模块等价物：显式路障，学员删掉它才算真正完成这一题。
inline void wait(std::source_location loc = std::source_location::current()) {
    d2x::report::wait(loc.file_name(), static_cast<int>(loc.line()));
    std::print("\033[33m[HONLY LOGW]: {}:{} - 🥳 Delete the d2x::wait() to continue...\033[0m\n",
               loc.file_name(), loc.line());
    std::fflush(stdout);
}

} // namespace d2x
