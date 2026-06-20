// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp14/01-relaxed-constexpr-0.cpp
//
// Exercise: cpp14 | 01 - relaxed constexpr | constexpr with loops
//
// Tips:
// - C++14 constexpr functions can use for / while loops
// - Loops may contain local variables and compound statements
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/constexpr
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/01-relaxed-constexpr.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker relaxed-constexpr
//

#include <d2x/cpp/common.hpp>

// constexpr factorial — C++14 allows a for loop
constexpr int factorial(int n) {
    D2X_YOUR_ANSWER result = 1;
    for (int i = 1; D2X_YOUR_ANSWER; ++i) {
        result *= i;
    }
    return result;
}

// constexpr power — while loop version
constexpr int power(int base, int exp) {
    int result = 1;
    int i = 0;
    while (D2X_YOUR_ANSWER) {
        result *= base;
        ++i;
    }
    return result;
}

int main() {

    // 0. for loop — compile-time factorial
    constexpr int f5 = factorial(5);
    static_assert(f5 == 120, "factorial(5) should be 120");
    d2x_assert_eq(f5, 120);

    constexpr int f0 = factorial(0);
    static_assert(f0 == 1, "factorial(0) should be 1");

    constexpr int f10 = factorial(10);
    d2x_assert_eq(f10, 3628800);

    // 1. while loop — compile-time power
    constexpr int p2 = power(2, 3);
    static_assert(p2 == 8, "power(2,3) should be 8");
    d2x_assert_eq(p2, D2X_YOUR_ANSWER);

    constexpr int p5 = power(5, 2);
    d2x_assert_eq(p5, 25);

    D2X_WAIT

    return 0;
}
