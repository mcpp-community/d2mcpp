// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/07-constexpr-0.cpp
//
// Exercise: cpp11 | 07 - constexpr | Compile-time computation basics: Difference between constexpr and const
//
// Tips: Fix compiler errors and understand compile-time variables and compile-time function computation
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/constexpr
//
// Auto-Checker command:
//
//   d2x checker constexpr
//

#include <d2x/cpp/common.hpp>

#include <iostream>

int sum_for_1_to(int n) {
    return n == 1 ? 1 : n + sum_for_1_to(n - 1);
}

int main() {

    { // 1. Runtime variables, constants, and compile-time variables
        int size1 = 10;
        const int size2 = size1 + 10;
        constexpr int size3 = 10 * 3;

        int arr1[size1]; // Choose the correct sizex as array size
    }

    { // 2. Compile-time computation basics
        constexpr int s = sum_for_1_to(4);
        d2x_assert_eq(s, 1 + 2 + 3 + 4);
    }

    D2X_WAIT

    return 0;
}