// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp14/01-relaxed-constexpr-1.cpp
//
// Exercise: cpp14 | 01 - relaxed constexpr | constexpr with branches and local variables
//
// Tips:
// - C++14 constexpr functions can use if / switch branches
// - Multiple local variables can be declared and used in constexpr
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

// constexpr absolute value — if branch
constexpr int my_abs(int x) {
    if (D2X_YOUR_ANSWER) {
        return -x;
    }
    return x;
}

// constexpr days in month — switch branch
constexpr int days_in_month(D2X_YOUR_ANSWER month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return 28;
        D2X_YOUR_ANSWER:
            return 0;
    }
}

// constexpr fibonacci — multiple local variables + loop
constexpr int fib(int n) {
    int a = 0, b = D2X_YOUR_ANSWER;
    for (int i = 0; i < n; ++i) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return D2X_YOUR_ANSWER;
}

int main() {

    // 0. if branch
    constexpr int a1 = my_abs(-10);
    static_assert(a1 == 10, "");
    d2x_assert_eq(a1, 10);

    constexpr int a2 = my_abs(0);
    d2x_assert_eq(a2, 0);

    // 1. switch branch
    constexpr int d7 = days_in_month(7);
    static_assert(d7 == 31, "July has 31 days");
    d2x_assert_eq(d7, 31);

    constexpr int d2 = days_in_month(2);
    d2x_assert_eq(d2, D2X_YOUR_ANSWER);

    // 2. multiple local variables + loop — fib
    constexpr int f10 = fib(10);
    static_assert(f10 == 55, "fib(10) should be 55");
    d2x_assert_eq(f10, 55);

    constexpr int f0 = fib(0);
    d2x_assert_eq(f0, 0);

    D2X_WAIT

    return 0;
}
