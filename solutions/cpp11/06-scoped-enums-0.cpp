// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/06-scoped-enums-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/06-scoped-enums-0.cpp
//
// 教学要点: 传统 enum 会污染外层作用域并隐式转 int, 这里通过重命名解决符号冲突,
// 保留并展示 "color == Apple" 这种本不该相等却被静默接受的逻辑错误。

#include <d2x/cpp/common.hpp>

#include <iostream>

enum Color {
    RED,
    GREEN,
    BLUE,
    ORANGE_COLOR // 1.类型冲突 - 橙色 (重命名以避免与 Fruit::ORANGE 冲突)
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
        // 代码会运行到这里 (传统枚举的潜在 bug)
        std::cout << "color == Apple ?!" << std::endl;
    }

    if (fruit == RED) {
        std::cout << "fruit == RED ?!" << std::endl;
    }

    return 0;
}
