// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-1.cpp
//
// Exercise: cpp11 | 00 - auto and decltype | Expression Type Deduction
//
// Tips: Use auto and decltype to fix errors in the code
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/auto
//   - https://en.cppreference.com/w/cpp/language/decltype
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/00-auto-and-decltype.md
//
// Forum: https://forum.d2learn.org/category/20
//
// Auto-Checker command:
//
//   d2x checker auto-and-decltype-1
//


#include <d2x/common.hpp>

int main() {

    // 1. Expressions
    int a = 1;
    auto a1 = a + 2;
    D2X_YOUR_ANSWER a2 = a + 2 + 1.1;

    int b = 2;
    D2X_YOUR_ANSWER b1 = a + 0.1;
    decltype(a + b + 1.1) b2 = a + b + 1.1;

    char c = 'c';
    D2X_YOUR_ANSWER c1 = 1 + c;
    D2X_YOUR_ANSWER c2 = 2 + 'a';

    d2x_assert_eq(a2, a + 2 + 1.1);
    d2x_assert_eq(b1, a + 0.1);
    d2x_assert_eq(c1, 1 + c);
    d2x_assert_eq(c2, 2 + 'a');

    D2X_WAIT

    return 0;
}
