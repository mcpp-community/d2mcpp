// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/02-final-and-override-0.cpp
//
// Exercise/练习: cpp11 | 02 - final and override
//
// Tips/提示: 修正代码中override的使用错误
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/final
//   - https://en.cppreference.com/w/cpp/language/override
//
// Auto-Checker/自动检测命令:
//
//   d2x checker final-and-override
//

#include <d2x/common.hpp>

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
    void func1()  {
        std::cout << "B::func1()" << std::endl;
    }

    void func2() override {
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

    D2X_WAIT

    return 0;
}