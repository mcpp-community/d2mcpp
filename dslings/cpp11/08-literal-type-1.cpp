// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/08-literal-type-1.cpp
//
// Exercise/练习: cpp11 | 08 - literal type | 自定义字面值类型
//
// Tips/提示: 根据编译器的输出, 修复编译器报错, 了解如何定义字面值类型
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/named_req/LiteralType.html
//
// Auto-Checker/自动检测命令:
//
//   d2x checker literal-type-1
//

#include <d2x/common.hpp>

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