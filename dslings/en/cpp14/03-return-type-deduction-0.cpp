// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp14/03-return-type-deduction-0.cpp
//
// Exercise: cpp14 | 03 - return type deduction | auto return type deduction
//
// Tips:
// - C++14 allows auto as a return type without trailing -> decltype(...)
// - The compiler deduces the return type from the return statement
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/auto
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/03-return-type-deduction.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker return-type-deduction
//

#include <d2x/cpp/common.hpp>
#include <string>

auto get_forty_two() {
    return D2X_YOUR_ANSWER;
}

auto greet(D2X_YOUR_ANSWER name) {
    return "hello " + D2X_YOUR_ANSWER;
}

template <typename T1, typename T2>
D2X_YOUR_ANSWER max_of(T1 a, T2 b) {
    return a > b ? a : b;
}

int main() {

    d2x_assert_eq(get_forty_two(), 42);

    d2x_assert(greet(std::string("world")) == "hello world");

    d2x_assert_eq(max_of(10, 20), 20);
    d2x_assert_eq(max_of(2.5, D2X_YOUR_ANSWER), 3.5);

    D2X_WAIT

    return 0;
}
