// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp14/02-variable-templates-1.cpp
//
// Exercise/练习: cpp14 | 02 - variable templates | _v 类型萃取
//
// Tips/提示:
// - _v 后缀的类型萃取本质就是变量模板, 省去了 ::value
// - template<typename T> constexpr bool xxx_v = xxx<T>::value;
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/variable_template
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/02-variable-templates.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker variable-templates
//

#include <d2x/cpp/common.hpp>
#include <type_traits>

template <typename T>
constexpr bool is_void_v = D2X_YOUR_ANSWER;

template <typename T, typename U>
constexpr D2X_YOUR_ANSWER is_same_v = std::is_same<T, U>::value;

template <int N>
constexpr int factorial_v = D2X_YOUR_ANSWER;

template <>
constexpr int factorial_v<0> = D2X_YOUR_ANSWER;

int main() {

    static_assert(is_void_v<void>, "");
    d2x_assert(is_void_v<void>);
    d2x_assert(!is_void_v<int>);

    static_assert(is_same_v<int, int>, "");
    d2x_assert((is_same_v<int, int>));
    d2x_assert((!is_same_v<int, float>));
    d2x_assert((is_same_v<D2X_YOUR_ANSWER, char>));

    static_assert(factorial_v<5> == 120, "");
    d2x_assert_eq(factorial_v<5>, 120);
    d2x_assert_eq(factorial_v<0>, 1);
    d2x_assert_eq(factorial_v<3>, 6);

    D2X_WAIT

    return 0;
}
