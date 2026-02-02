// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/06-scoped-enums-0.cpp
//
// Exercise/练习: cpp11 | 06 - scoped enums | 传统枚举类型潜在问题
//
// Tips/提示: 通过编译器的错误提示修复代码, 并理解传统枚举类型的潜在问题
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/enum
//
// Auto-Checker/自动检测命令:
//
//   d2x checker scoped-enums
//

#include <d2x/cpp/common.hpp>

#include <iostream>

enum Color {
    RED,
    GREEN,
    BLUE,
    ORANGE // 1.类型冲突 - 橙色
};

enum Fruit {
    Apple,
    Banana,
    ORANGE // 1.类型冲突 - 橙子
};

int main() {
 
    Color color = RED;
    Fruit fruit = Apple;

    d2x_assert_eq(color, RED);
    d2x_assert_eq(fruit, Apple);

    // 2.符合语法, 但逻辑错误的类型匹配
    if (color == Apple) { // 不要删除这行代码
        // 代码会运行到这里
        D2X_WAIT
    }

    if (fruit == RED) {
        D2X_WAIT
    }

    D2X_WAIT

    return 0;
}