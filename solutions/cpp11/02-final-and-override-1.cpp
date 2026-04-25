// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/02-final-and-override-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/02-final-and-override-1.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

struct A {
    // 移除 final, 否则 B 不能 override
    virtual int func1() {
       return 1;
    }
    int func2() { return 2; }
};

// 移除 struct B 的 final, 否则 C 不能继承 B
struct B : A  {

    int func1() override {
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

    return 0;
}
