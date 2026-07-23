// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/02-final-and-override/0.cpp
//
// Exercise: cpp11 | 02 - final and override
//
// Tips: Correct the usage errors of override in the code
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/final
//   - https://en.cppreference.com/w/cpp/language/override
//
// Auto-Checker command:
//
//   d2x checker final-and-override
//

import std;
import d2x;


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

    B override; // Do not directly modify the code in the main function
    override.func1(); // B::func1()
    override.func2(); // B::func2()

    A *a = &override;
    a->func1(); // B::func1()
    a->func2(); // A::func2()

    d2x::wait();
    return 0;
}