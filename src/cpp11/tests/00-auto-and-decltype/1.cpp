// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/cpp11/tests/00-auto-and-decltype/1.cpp
//
// Exercise/练习: cpp11 | 00 - auto and decltype | 表达式类型推导
//
// Tips/提示: 含浮点的表达式要推导成 double, 别用 int 截断了精度
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


import std;
import d2x;

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

    d2x::check_eq(a2, a + 2 + 1.1, "a2 == a + 2 + 1.1");
    d2x::check_eq(b1, a + 0.1, "b1 == a + 0.1");
    d2x::check_eq(c1, 1 + c, "c1 == 1 + c");
    d2x::check_eq(c2, 2 + 'a', "c2 == 2 + 'a'");

    d2x::wait();
    return 0;
}