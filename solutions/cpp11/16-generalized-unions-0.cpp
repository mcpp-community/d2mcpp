// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/16-generalized-unions-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/16-generalized-unions-0.cpp
//

#include <d2x/cpp/common.hpp>


union M
{
    int a1 = 42;     // 仅允许一个成员有默认初始化
    int a2;
    double a3;
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

    return 0;
}
