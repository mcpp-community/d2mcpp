// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/08-literal-type-1.cpp
//
// Exercise: cpp11 | 08 - literal type | Custom literal types
//
// Tips: Fix compiler errors based on compiler output, understand how to define literal types
//
// Docs:
//   - https://en.cppreference.com/w/cpp/named_req/LiteralType.html
//
// Auto-Checker command:
//
//   d2x checker literal-type-1
//

#include <d2x/cpp/common.hpp>

#include <iostream>

struct Vector {
    int x, y;
    Vector(int x_, int y_) : x(x_), y(y_) { }
};

constexpr Vector add(const Vector& v1, const Vector& v2) {
    return Vector(v1.x + v2.x, v1.y + v2.y);
}

int main() {

    constexpr Vector v1{1, 2}, v2{2, 3};
    constexpr Vector v3 = add(v1, v2);

    std::cout << "[ " << v3.x << ", " << v3.y << " ]" << std::endl;

    D2X_WAIT

    return 0;
}