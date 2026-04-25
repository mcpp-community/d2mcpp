// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/00-auto-and-decltype-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/00-auto-and-decltype-1.cpp
//

#include <d2x/cpp/common.hpp>

int main() {

    // 1. 表达式
    int a = 1;
    auto a1 = a + 2;
    auto a2 = a + 2 + 1.1;       // double

    int b = 2;
    decltype(a + 0.1) b1 = a + 0.1; // double
    decltype(a + b + 1.1) b2 = a + b + 1.1;
    (void)a1; (void)b2;

    char c = 'c';
    auto c1 = 1 + c;             // int
    decltype(2 + 'a') c2 = 2 + 'a';

    d2x_assert_eq(a2, a + 2 + 1.1);
    d2x_assert_eq(b1, a + 0.1);
    d2x_assert_eq(c1, 1 + c);
    d2x_assert_eq(c2, 2 + 'a');

    return 0;
}
