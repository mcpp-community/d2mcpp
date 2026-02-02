// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-0.cpp
//
// Exercise: cpp11 | 00 - auto and decltype | Automatic Type Deduction
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
//   d2x checker auto-and-decltype
//

#include <d2x/cpp/common.hpp>

int main() {

    // 0. Declaration and definition
    int a = 1;
    auto a1 = a; // a1's type is int
    int b = 2;
    D2X_YOUR_ANSWER b1 = b;

    decltype(b) b2 = b; // b2's type is int
    D2X_YOUR_ANSWER a2 = a;

    char c = 'c';
    D2X_YOUR_ANSWER c1 = c;
    D2X_YOUR_ANSWER c2 = c;

    d2x_assert_eq(a, a1);
    d2x_assert_eq(a1, a2);
    d2x_assert_eq(b, b1);
    d2x_assert_eq(b1, b2);

    D2X_WAIT

    return 0;
}
