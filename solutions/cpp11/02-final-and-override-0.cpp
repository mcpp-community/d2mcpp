// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/02-final-and-override-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/02-final-and-override-0.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

struct A {
    virtual void func1() {
        std::cout << "A::func1()" << std::endl;
    }

    void func2() {
        std::cout << "A::func2()" << std::endl;
    }
};

struct B : A {
    void func1() override {
        std::cout << "B::func1()" << std::endl;
    }

    // func2 不是 virtual, 不能 override
    void func2() {
        std::cout << "B::func2()" << std::endl;
    }
};


int main() {

    B override; // 不要直接修改main函数中的代码
    override.func1(); // B::func1()
    override.func2(); // B::func2()

    A *a = &override;
    a->func1(); // B::func1()
    a->func2(); // A::func2()

    return 0;
}
