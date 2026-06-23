// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/02-variable-templates-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/02-variable-templates-1.cpp
//

#include <d2x/cpp/common.hpp>
#include <type_traits>

template <typename T>
constexpr bool is_void_v = std::is_void<T>::value;

template <typename T, typename U>
constexpr bool is_same_v = std::is_same<T, U>::value;

template <int N>
constexpr int factorial_v = N * factorial_v<N - 1>;

template <>
constexpr int factorial_v<0> = 1;

int main() {

    static_assert(is_void_v<void>, "");
    d2x_assert(is_void_v<void>);
    d2x_assert(!is_void_v<int>);

    static_assert(is_same_v<int, int>, "");
    d2x_assert((is_same_v<int, int>));
    d2x_assert((!is_same_v<int, float>));
    d2x_assert((is_same_v<char, char>));

    static_assert(factorial_v<5> == 120, "");
    d2x_assert_eq(factorial_v<5>, 120);
    d2x_assert_eq(factorial_v<0>, 1);
    d2x_assert_eq(factorial_v<3>, 6);

    return 0;
}
