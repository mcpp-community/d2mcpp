// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/00-auto-and-decltype-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/00-auto-and-decltype-0.cpp
//

import std;
import d2x;

int main() {

    // 0. 声明定义
    int a = 1;
    auto a1 = a;          // a1: int
    int b = 2;
    auto b1 = b;          // b1: int

    decltype(b) b2 = b;   // b2: int
    decltype(a) a2 = a;   // a2: int

    char c = 'c';
    auto c1 = c;          // c1: char
    decltype(c) c2 = c;   // c2: char

    d2x::check_eq(a, a1, "a == a1");
    d2x::check_eq(a1, a2, "a1 == a2");
    d2x::check_eq(b, b1, "b == b1");
    d2x::check_eq(b1, b2, "b1 == b2");
    d2x::check_eq(c, c1, "c == c1");
    d2x::check_eq(c1, c2, "c1 == c2");

    return 0;
}
