// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-1.cpp
//
// Exercise/练习: cpp11 | 00 - auto and decltype | 表达式类型推导
//
// Tips/提示: 使用 auto 和 decltype 修复代码中的错误
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
//   d2x checker auto-and-decltype-1
//


#include <d2x/common.hpp>

int main() {

    // 1. 表达式
    int a = 1;
    auto a1 = a + 2;
    D2X_YOUR_ANSWER a2 = a + 2 + 1.1;

    int b = 2;
    D2X_YOUR_ANSWER b1 = a + 0.1;
    decltype(a + b + 1.1) b2 = a + b + 1.1;

    char c = 'c';
    D2X_YOUR_ANSWER c1 = 1 + c;
    D2X_YOUR_ANSWER c2 = 2 + 'a';

    d2x_assert_eq(a2, a + 2 + 1.1);
    d2x_assert_eq(b1, a + 0.1);
    d2x_assert_eq(c1, 1 + c);
    d2x_assert_eq(c2, 2 + 'a');

    D2X_WAIT

    return 0;
}