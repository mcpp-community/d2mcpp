// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/08-literal-type-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/08-literal-type-1.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>

struct Vector {
    int x, y;
    constexpr Vector(int x_, int y_) : x(x_), y(y_) { }
};

constexpr Vector add(const Vector& v1, const Vector& v2) {
    return Vector(v1.x + v2.x, v1.y + v2.y);
}

int main() {

    constexpr Vector v1{1, 2}, v2{2, 3};
    constexpr Vector v3 = add(v1, v2);

    std::cout << "[ " << v3.x << ", " << v3.y << " ]" << std::endl;

    return 0;
}
