// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/07-constexpr-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/07-constexpr-0.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>

constexpr int sum_for_1_to(int n) {
    return n == 1 ? 1 : n + sum_for_1_to(n - 1);
}

int main() {

    { // 1. 运行时变量、常量和编译期变量
        int size1 = 10;
        const int size2 = size1 + 10;
        constexpr int size3 = 10 * 3;
        (void)size1; (void)size2;

        // 选择 constexpr 的 size3 作为编译期数组大小
        int arr1[size3];
        (void)arr1;
    }

    { // 2. 编译期计算基础
        constexpr int s = sum_for_1_to(4);
        d2x_assert_eq(s, 1 + 2 + 3 + 4);
    }

    return 0;
}
