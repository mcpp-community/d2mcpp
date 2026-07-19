#ifndef COMMON_HPP_D2X
#define COMMON_HPP_D2X

// d2mcpp 练习脚手架 —— 经典 #include 路径。
//
// 这条路径提供宏，因为 D2X_YOUR_ANSWER 必须展开为空才能制造编译错误，
// 而宏无法跨模块导出。模块化的练习请改用 `import d2x.harness;`，它提供
// d2x::check / check_eq / wait（靠 std::source_location 自动带 file/line），
// 但没有填空占位符的等价物。
//
// 可见输出与旧版逐字节一致 —— ✅/❌ 的逐条对照是教学的一部分。新增的只是
// 把同样的结果写进侧信道，让判定不再依赖扫 stdout 找 emoji（那会误判：
// 一个断言全过的正确解答，只要在说明文字里打了个 ❌ 就被判失败）。

#include <iostream>
#include <string>

#include <d2x/cpp/honly_logger.hpp>
#include <d2x/cpp/report.hpp>

namespace d2x::detail {

// std::to_string 只接受算术类型，但断言里允许任意可比较类型。
// 转得了的转，转不了的留空：侧信道少一点细节，好过整个 harness 编不过。
template<typename T>
inline std::string show(const T& v) {
    if constexpr (requires { std::to_string(v); }) return std::to_string(v);
    else if constexpr (requires { std::string(v); }) return std::string(v);
    else return {};
}

} // namespace d2x::detail

#define d2x_assert(expr) \
{ \
    bool d2x_ok_ = static_cast<bool>(expr); \
    d2x::report::assertion(d2x_ok_, #expr, "true", d2x_ok_ ? "true" : "false", \
                           __FILE__, __LINE__); \
    if (!d2x_ok_) { \
        HONLY_LOGW("❌(error) | %s", #expr); \
    } else { \
        HONLY_LOGI_P("✅ | %s", #expr); \
    } \
}

#define d2x_assert_eq(a, b) \
{ \
    bool d2x_ok_ = ((a) == (b)); \
    d2x::report::assertion(d2x_ok_, #a " == " #b, \
                           d2x::detail::show(b), d2x::detail::show(a), \
                           __FILE__, __LINE__); \
    if (!d2x_ok_) {\
        HONLY_LOGW("❌ | %s == %s (%s == %s)", \
            #a, #b, d2x::detail::show(a).c_str(), d2x::detail::show(b).c_str()); \
    } else {\
        HONLY_LOGI_P("✅ | %s == %s (%s == %s)", \
            #a, #b, d2x::detail::show(a).c_str(), d2x::detail::show(b).c_str()); \
    } \
}

#define D2X_WAIT { \
    d2x::report::wait(__FILE__, __LINE__); \
    HONLY_LOGW("🥳 Delete the D2X_WAIT to continue..."); \
}

// 展开为空 —— 学员在这里填类型。留空会得到「未声明」错误，
// 比留一个占位标识符导致的语法错误更贴近意图。
#define D2X_YOUR_ANSWER

#define D2X_DONT_DELETE_THIS(x) x

#endif // COMMON_HPP_D2X
