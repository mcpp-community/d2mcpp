// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp14/04-decltype-auto-1.cpp
//
// Exercise: cpp14 | 04 - decltype(auto) | parentheses trap and variable declarations
//
// Tips:
// - decltype(auto) in variable declarations follows decltype deduction rules
// - return x and return (x) deduce differently under decltype(auto)
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/auto
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/04-decltype-auto.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker decltype-auto
//

#include <d2x/cpp/common.hpp>

int val = 50;

// return name → value type
decltype(auto) return_name() {
    return D2X_YOUR_ANSWER;
}

// return (name) → reference type
decltype(auto) return_paren() {
    return (D2X_YOUR_ANSWER);
}

int main() {

    // 0. decltype(auto) in variable declarations
    D2X_YOUR_ANSWER v1 = val;
    v1 = 99;
    d2x_assert_eq(val, 50);

    decltype(auto) v2 = (D2X_YOUR_ANSWER);
    v2 = 99;
    d2x_assert_eq(val, 99);

    // 1. return_name returns by value, modifications don't affect original
    auto a1 = return_name();
    d2x_assert_eq(a1, 50);
    ++a1;
    d2x_assert_eq(val, 99);

    // 2. return_paren returns by reference, modifications affect original
    auto& a2 = return_paren();
    d2x_assert_eq(a2, D2X_YOUR_ANSWER);
    ++a2;
    d2x_assert_eq(val, 100);

    D2X_WAIT

    return 0;
}
