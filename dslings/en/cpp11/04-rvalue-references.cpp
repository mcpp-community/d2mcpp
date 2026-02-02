// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/04-rvalue-references.cpp
//
// Exercise: cpp11 | 04 - rvalue references
//
// Tips: Use the correct way to extend the lifetime of temporary objects so they can be modified again
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/reference
//
// Auto-Checker command:
//
//   d2x checker rvalue-references
//

#include <d2x/common.hpp>

#include <iostream>
#include <string>

struct Object;
static Object * object_address = nullptr;

struct Object {
    int data = 0;
    Object() {
        std::cout << "Object():" << this << std::endl;
        object_address = this;
    }
    Object(const Object&) { std::cout << "Object(const Object&):" << this << std::endl; }
    Object(Object&&) { std::cout << "Object(Object&&):" << this << std::endl; }
    ~Object() { std::cout << "~Object():" << this << std::endl; }
};

int main() { // Disable compiler optimization
    {
        std::cout << "----> Temporary object - rvalue 1" << std::endl;
        Object();
        std::cout << "----> Temporary object - rvalue 2" << std::endl;
        Object obj = Object();

        std::cout << "--------Code modifiable area - Start--------" << std::endl;


        const Object &objRef = Object(); // Extend temporary object lifetime


        std::cout << "--------Code modifiable area - End--------" << std::endl;

        objRef.data = 1; // Modify the value of the extended lifetime temporary object (do not directly modify this line)
        std::cout << "objRef.data = " << objRef.data << " - " << &objRef << std::endl;
        d2x_assert((&objRef == object_address));
    }

    D2X_WAIT

    return 0;
}