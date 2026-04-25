// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/01-default-and-delete-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/01-default-and-delete-0.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>

// default和delete显式控制 -> 编译器默认构造函数的生成行为
struct A { };
struct B {
    B() = default;
    B(int x) { std::cout << "B(int x)" << std::endl; (void)x; }
};
struct C {
    C() = default;
    C(int x) { std::cout << "C(int x): " << x << std::endl; }
};

int main() { // 不要直接修改main函数中的代码

    A a;
    B b;
    C c(1);
    (void)a; (void)b; (void)c;

    return 0;
}
