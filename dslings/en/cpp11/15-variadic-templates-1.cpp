// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/15-variadic-templates-1.cpp
//
// Exercise: cpp11 | 15 - variadic templates | Variadic template sum
//
// Tips:
// - Implement the sum function using recursion
// - Handle the recursion terminating condition as well
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/parameter_pack
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/15-variadic-templates.md
//

#include <d2x/cpp/common.hpp>

D2X_YOUR_ANSWER

template<typename T, D2X_YOUR_ANSWER>
T sum(T first, D2X_YOUR_ANSWER) {
    return first + sum(D2X_YOUR_ANSWER);
}

int main() {
    int res1 = sum(1, 2, 3, 4, 5);
    d2x_assert_eq(res1, 15);

    double res2 = sum(1.5, 2.5, 3.0);
    d2x_assert(res2 == 7.0);

    // Mixed types
    // Note: the return type is determined by the first argument T
    int res3 = sum(10, 20.5);
    d2x_assert_eq(res3, 30);

    D2X_WAIT

    return 0;
}
