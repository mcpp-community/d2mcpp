// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/01-relaxed-constexpr-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/01-relaxed-constexpr-0.cpp
//

#include <d2x/cpp/common.hpp>

constexpr int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

constexpr int power(int base, int exp) {
    int result = 1;
    int i = 0;
    while (i < exp) {
        result *= base;
        ++i;
    }
    return result;
}

int main() {

    constexpr int f5 = factorial(5);
    static_assert(f5 == 120, "factorial(5) should be 120");
    d2x_assert_eq(f5, 120);

    constexpr int f0 = factorial(0);
    static_assert(f0 == 1, "factorial(0) should be 1");

    constexpr int f10 = factorial(10);
    d2x_assert_eq(f10, 3628800);

    constexpr int p2 = power(2, 3);
    static_assert(p2 == 8, "power(2,3) should be 8");
    d2x_assert_eq(p2, 8);

    constexpr int p5 = power(5, 2);
    d2x_assert_eq(p5, 25);

    return 0;
}
