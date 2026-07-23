// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/cpp11/tests/00-auto-and-decltype/0.cpp
//
// Exercise/练习: cpp11 | 00 - auto and decltype | 自动类型推导
//
// Tips/提示: 用 auto 从初始值推导类型, 用 decltype 取已声明变量的类型
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
//   d2x checker auto-and-decltype
//

import std;
import d2x;

int main() {

    // 0. 声明定义
    int a = 1;
    auto a1 = a; // a1 的类型是 int
    int b = 2;
    D2X_YOUR_ANSWER b1 = b;

    decltype(b) b2 = b; // b2 的类型是 int
    D2X_YOUR_ANSWER a2 = a;

    char c = 'c';
    D2X_YOUR_ANSWER c1 = c;
    D2X_YOUR_ANSWER c2 = c;

    d2x::check_eq(a, a1, "a == a1");
    d2x::check_eq(a1, a2, "a1 == a2");
    d2x::check_eq(b, b1, "b == b1");
    d2x::check_eq(b1, b2, "b1 == b2");
    d2x::check_eq(c, c1, "c == c1");
    d2x::check_eq(c1, c2, "c1 == c2");

    d2x::wait();
    return 0;
}