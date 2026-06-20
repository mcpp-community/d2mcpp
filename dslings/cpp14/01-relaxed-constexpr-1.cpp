// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp14/01-relaxed-constexpr-1.cpp
//
// Exercise/练习: cpp14 | 01 - relaxed constexpr | constexpr 分支与局部变量
//
// Tips/提示:
// - C++14 constexpr 函数可以使用 if / switch 分支
// - 可以在 constexpr 中声明和使用多个局部变量
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/constexpr
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/01-relaxed-constexpr.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker relaxed-constexpr
//

#include <d2x/cpp/common.hpp>

// constexpr 绝对值 — if 分支
constexpr int my_abs(int x) {
    if (D2X_YOUR_ANSWER) {
        return -x;
    }
    return x;
}

// constexpr 月份天数 — switch 分支
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

// constexpr 斐波那契 — 多局部变量 + 循环
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

    // 0. if 分支
    constexpr int a1 = my_abs(-10);
    static_assert(a1 == 10, "");
    d2x_assert_eq(a1, 10);

    constexpr int a2 = my_abs(0);
    d2x_assert_eq(a2, 0);

    // 1. switch 分支
    constexpr int d7 = days_in_month(7);
    static_assert(d7 == 31, "July has 31 days");
    d2x_assert_eq(d7, 31);

    constexpr int d2 = days_in_month(2);
    d2x_assert_eq(d2, D2X_YOUR_ANSWER);

    // 2. 多局部变量 + 循环 — fib
    constexpr int f10 = fib(10);
    static_assert(f10 == 55, "fib(10) should be 55");
    d2x_assert_eq(f10, 55);

    constexpr int f0 = fib(0);
    d2x_assert_eq(f0, 0);

    D2X_WAIT

    return 0;
}
