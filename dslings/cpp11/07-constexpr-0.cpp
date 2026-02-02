// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/07-constexpr-0.cpp
//
// Exercise/练习: cpp11 | 07 - constexpr | 编译期计算基础: constexpr 和 const的区别
//
// Tips/提示: 修复编译器报错, 并了解编译器变量和编译期函数计算
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/constexpr
//
// Auto-Checker/自动检测命令:
//
//   d2x checker constexpr
//

#include <d2x/cpp/common.hpp>

#include <iostream>

int sum_for_1_to(int n) {
    return n == 1 ? 1 : n + sum_for_1_to(n - 1);
}

int main() {

    { // 1. 运行时变量、常量和编译期变量
        int size1 = 10;
        const int size2 = size1 + 10;
        constexpr int size3 = 10 * 3;

        int arr1[size1]; // 选择正确的sizex作为数组大小
    }

    { // 2. 编译期计算基础
        constexpr int s = sum_for_1_to(4);
        d2x_assert_eq(s, 1 + 2 + 3 + 4);
    }

    D2X_WAIT

    return 0;
}