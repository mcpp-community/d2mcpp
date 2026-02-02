// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/09-list-initialization-1.cpp
//
// Exercise: cpp11 | 09 - list initialization | Default initialization syntax pitfalls
//
// Tips: Fix compiler errors based on compiler output, understand default initialization syntax pitfalls
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/list_initialization.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/09-list-initialization.md
//
// Auto-Checker command:
//
//   d2x checker list-initialization
//

#include <d2x/cpp/common.hpp>

#include <iostream>

struct Object {
    Object() {
        x = 0;
        std::cout << "Object()" << std::endl;
    }
    Object(int x) : x(x) {
        std::cout << "Object(int): " << x << std::endl;
    }
    int x;
};

int main() {

    Object obj1();
    Object obj2(2);

    d2x_assert_eq(obj1.x, 0);
    d2x_assert_eq(obj2.x, 2);

    D2X_WAIT

    return 0;
}