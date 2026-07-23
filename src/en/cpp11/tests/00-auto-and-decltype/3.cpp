// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/00-auto-and-decltype/3.cpp
//
// Exercise: cpp11 | 00 - auto and decltype | Function Return Type Deduction
//
// Tips: Use a trailing return auto ... -> decltype(a - b) so the template function deduces its return type
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
//   d2x checker auto-and-decltype-3
//

import std;
import d2x;


// 3. Function return types

auto add_func(int a, double b) -> decltype(a + b) {
    return a + b;
}

template<typename T1, typename T2>
D2X_YOUR_ANSWER minus_func(T1 a, T2 b) -> D2X_YOUR_ANSWER {
    return a - b;
}

int main() {

    d2x::check_eq(minus_func(1, 2), -1, "minus_func(1, 2) == -1");
    d2x::check_eq(minus_func(2, 1), 1, "minus_func(2, 1) == 1");
    d2x::check_eq(minus_func(1, 2.1), -1.1, "minus_func(1, 2.1) == -1.1");

    d2x::wait();
    return 0;
}
