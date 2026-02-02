// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/01-default-and-delete-0.cpp
//
// Exercise: cpp11 | 01 - default and delete | Explicitly specifying constructor generation behavior
//
// Tips: Use `= default` and `= delete` to fix errors based on compiler hints
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/function#Function_definition
//   - https://en.cppreference.com/w/cpp/language/function#Deleted_functions
//
// Auto-Checker command:
//
//   d2x checker default-and-delete
//

#include <d2x/cpp/common.hpp>

#include <iostream>

// default and delete explicitly control -> compiler default constructor generation behavior
struct A { };
struct B {
    B(int x) { std::cout << "B(int x)" << std::endl; }
};
struct C {
    C() { }
    C(int x = 1) { std::cout << "C(int x = 1)" << std::endl; }
};

int main() { // Do not directly modify the code in the main function

    A a;
    B b;
    C c(1);

    D2X_WAIT

    return 0;
}