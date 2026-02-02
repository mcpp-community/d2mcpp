// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/01-default-and-delete-2.cpp
//
// Exercise: cpp11 | 01 - default and delete | Disabling function overloading
//
// Tips: Fix compiler errors, delete incorrect function calls
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/function#Function_definition
//   - https://en.cppreference.com/w/cpp/language/function#Deleted_functions
//
// Auto-Checker command:
//
//   d2x checker default-and-delete-2
//

#include <d2x/cpp/common.hpp>

#include <iostream>

void func(int x) {
    std::cout << "x = " << x << std::endl;
}

// Explicitly delete float parameter overload
void func(float) = delete;

int main() {

    func(1);     // int
    func(1.1f);  // float

    D2X_WAIT

    return 0;
}