// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/09-list-initialization-0.cpp
//
// Exercise: cpp11 | 09 - list initialization | Narrowing checks
//
// Tips: Fix compiler errors based on compiler output, understand narrowing checks in list initialization
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/list_initialization.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/09-list-initialization.md
//
// Auto-Checker command:
//
//   d2x checker list-initialization
//

#include <d2x/common.hpp>

#include <iostream>

int main() {

    int a1 = 1.1;
    d2x_assert_eq(a1, 1);

    int a2 = 1.1;
    int a3 = { 1.1 };
    d2x_assert_eq(a2, 1);
    d2x_assert_eq(a3, 1);

    double b1 { 1.1 };
    constexpr double c1 { 2.2 };

    int b2 { b1 };
    int c2 { c1 };

    d2x_assert_eq(b2, 1);
    d2x_assert_eq(c2, 2);

    int arr1[] = { 1, 2.2, 3 };
    d2x_assert_eq(arr1[1], 2);

    int arr2[4] { 1, b1, c1 };
    d2x_assert_eq(arr2[1], 1);
    d2x_assert_eq(arr2[2], 2);
    d2x_assert_eq(arr2[3], 0);

    D2X_WAIT

    return 0;
}