// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-3.cpp
//
// Exercise/练习: cpp11 | 00 - auto and decltype | 函数返回值类型推导
//
// Tips/提示: 用后置返回 auto ... -> decltype(a - b) 让模板函数自动推导返回类型
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/auto
//   - https://en.cppreference.com/w/cpp/language/decltype
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/00-auto-and-decltype.md
//
// 练习交流讨论: http://forum.d2learn.org/post/357
//
// Auto-Checker/自动检测命令:
//
//   d2x checker auto-and-decltype-3
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <vector>

// 3. 函数返回值类型

auto add_func(int a, double b) -> decltype(a + b) {
    return a + b;
}

template<typename T1, typename T2>
D2X_YOUR_ANSWER minus_func(T1 a, T2 b) -> D2X_YOUR_ANSWER {
    return a - b;
}

int main() {

    d2x_assert_eq(minus_func(1, 2), -1);
    d2x_assert_eq(minus_func(2, 1), 1);
    d2x_assert_eq(minus_func(1, 2.1), -1.1);

    D2X_WAIT

    return 0;
}