// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/02-final-and-override-1.cpp
//
// Exercise/练习: cpp11 | 02 - final and override
//
// Tips/提示: 修正代码中final的使用错误问题
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/final
//   - https://en.cppreference.com/w/cpp/language/override
//
// Auto-Checker/自动检测命令:
//
//   d2x checker final-and-override-1
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

struct A {
    virtual int func1() {
       return 1;
    }
    int func2() { return 2; }
};

struct B : A  {

    int func1() {
        return 3;
    }

    int func2() {
        return 4;
    }
};

struct C : B {

};

int main() {

    B final; // 不要直接修改main函数中的代码
    d2x_assert_eq(final.func1(), 3); // B::func1()
    d2x_assert_eq(final.func2(), 4); // B::func2()

    A *a = &final;
    d2x_assert_eq(a->func1(), 3); // B::func1()
    d2x_assert_eq(a->func2(), 2); // A::func2()

    D2X_WAIT

    return 0;
}