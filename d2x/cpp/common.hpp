#ifndef COMMON_HPP_D2X
#define COMMON_HPP_D2X

#include <iostream>
#include <string>

#include <d2x/cpp/honly_logger.hpp>

#define d2x_assert(expr) \
{ \
    if (!(expr)) { \
        HONLY_LOGW("❌(error) | %s", #expr); \
    } else { \
        HONLY_LOGI_P("✅ | %s", #expr); \
    } \
}

#define d2x_assert_eq(a, b) \
{ \
    if (a != b) {\
        HONLY_LOGW("❌ | %s == %s (%s == %s)", \
            #a, #b, std::to_string(a).c_str(), std::to_string(b).c_str()); \
    } else {\
        HONLY_LOGI_P("✅ | %s == %s (%s == %s)", \
            #a, #b, std::to_string(a).c_str(), std::to_string(b).c_str()); \
    } \
}

#define D2X_WAIT HONLY_LOGW("🥳 Delete the D2X_WAIT to continue...");
#define D2X_YOUR_ANSWER
#define D2X_DONT_DELETE_THIS(x) x

template<typename F, typename... Args>
class d2x_is_invocable {
private:
    template<typename U>
    static auto test(U* f) -> decltype((*f)(std::declval<Args>()...), std::true_type());

    template<typename>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<F>(nullptr))::value;
};

#endif