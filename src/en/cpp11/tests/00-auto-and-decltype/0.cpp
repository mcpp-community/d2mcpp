// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/00-auto-and-decltype/0.cpp
//
// Exercise: cpp11 | 00 - auto and decltype | Automatic Type Deduction
//
// Tips: Use auto to deduce from the initializer, and decltype to take an already-declared variable's type
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

import std;
import d2x;

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

    d2x::check_eq(a, a1, "a == a1");
    d2x::check_eq(a1, a2, "a1 == a2");
    d2x::check_eq(b, b1, "b == b1");
    d2x::check_eq(b1, b2, "b1 == b2");
    d2x::check_eq(c, c1, "c == c1");
    d2x::check_eq(c1, c2, "c1 == c2");

    d2x::wait();
    return 0;
}
