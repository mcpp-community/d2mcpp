// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/01-relaxed-constexpr-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/01-relaxed-constexpr-1.cpp
//

#include <d2x/cpp/common.hpp>

constexpr int my_abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

constexpr int days_in_month(int month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return 28;
        default:
            return 0;
    }
}

constexpr int fib(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

int main() {

    constexpr int a1 = my_abs(-10);
    static_assert(a1 == 10, "");
    d2x_assert_eq(a1, 10);

    constexpr int a2 = my_abs(0);
    d2x_assert_eq(a2, 0);

    constexpr int d7 = days_in_month(7);
    static_assert(d7 == 31, "July has 31 days");
    d2x_assert_eq(d7, 31);

    constexpr int d2 = days_in_month(2);
    d2x_assert_eq(d2, 28);

    constexpr int f10 = fib(10);
    static_assert(f10 == 55, "fib(10) should be 55");
    d2x_assert_eq(f10, 55);

    constexpr int f0 = fib(0);
    d2x_assert_eq(f0, 0);

    return 0;
}
