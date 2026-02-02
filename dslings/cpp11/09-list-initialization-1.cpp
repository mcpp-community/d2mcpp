// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/09-list-initialization-1.cpp
//
// Exercise/练习: cpp11 | 09 - list initialization | 默认初始化语法陷阱
//
// Tips/提示: 根据编译器的输出, 修复编译器报错, 了解默认初始化语法陷阱
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/list_initialization.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/09-list-initialization.md
//
// Auto-Checker/自动检测命令:
//
//   d2x checker list-initialization
//

#include <d2x/common.hpp>

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