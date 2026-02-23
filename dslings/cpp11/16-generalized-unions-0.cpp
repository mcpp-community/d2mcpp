// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/16-generalized-unions-0.cpp
//
// Exercise/练习: cpp11 | 16 - generalized unions | 广义非平凡联合体
//
// Tips/提示:
// - 允许初始化一个成员变量
//
// Docs/文档:
//   - https://cppreference.com/w/cpp/language/union.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/16-generalized-unions.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker generalized-unions
//

#include <d2x/cpp/common.hpp>


union M
{
    int a1 = D2X_YOUR_ANSWER;
    int a2 = 21;    // 移除多余的初始化
    D2X_YOUR_ANSWER a3;
    char c;
};


int main() {

    M u1;
    //初始化成员
    d2x_assert(u1.a1 == 42);
    u1.a2 = 21;

    double val = 3.14;
    u1.a3 = val;

    u1.c = 'x';

    D2X_WAIT

    return 0;
}
