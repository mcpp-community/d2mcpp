// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/09-list-initialization-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/09-list-initialization-0.cpp
//
// 教学要点: 大括号(列表)初始化对窄化转换不允许, 需要使用显式 static_cast 修复。

#include <d2x/cpp/common.hpp>

#include <iostream>

int main() {

    int a1 = 1.1; // 拷贝初始化允许窄化, 截断为 1
    d2x_assert_eq(a1, 1);

    int a2 = 1.1;
    int a3 = { static_cast<int>(1.1) }; // 大括号初始化要求显式转换
    d2x_assert_eq(a2, 1);
    d2x_assert_eq(a3, 1);

    double b1 { 1.1 };
    constexpr double c1 { 2.2 };

    int b2 { static_cast<int>(b1) };
    int c2 { static_cast<int>(c1) };

    d2x_assert_eq(b2, 1);
    d2x_assert_eq(c2, 2);

    int arr1[] = { 1, static_cast<int>(2.2), 3 };
    d2x_assert_eq(arr1[1], 2);

    int arr2[4] { 1, static_cast<int>(b1), static_cast<int>(c1) };
    d2x_assert_eq(arr2[1], 1);
    d2x_assert_eq(arr2[2], 2);
    d2x_assert_eq(arr2[3], 0);

    return 0;
}
