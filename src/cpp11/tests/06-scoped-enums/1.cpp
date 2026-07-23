// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/cpp11/tests/06-scoped-enums/1.cpp
//
// Exercise/练习: cpp11 | 06 - scoped enums | 范围枚举类型基本用法
//
// Tips/提示: 通过编译器的错误提示修复代码, 并理解传统枚举类型的潜在问题
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/enum
//
// Auto-Checker/自动检测命令:
//
//   d2x checker scoped-enums-1
//

import std;
import d2x;


enum class Color {
    RED,
    GREEN,
    BLUE,
    ORANGE // 橙色
};

enum Fruit {
    Apple,
    Banana,
    ORANGE // 橙子
};

int main() {

    // 1.作用域限定: 使用范围枚举类型, 解决ORANGE类型冲突问题
    Color color = Color::ORANGE;
    Fruit fruit = Fruit::ORANGE;

    d2x::check(color == Color::ORANGE, "color == Color::ORANGE");
    d2x::check(fruit == Fruit::ORANGE, "fruit == Fruit::ORANGE");

    // 2.类型安全: 防止不同类型的枚举值之间的比较
    if (color == Fruit::ORANGE) { // 使用Color类型修复编译错误
        d2x::check(color == Color::ORANGE, "color == Color::ORANGE");
    }

    // 3.类型检查: 默认情况下, 范围枚举类型的值是不可隐式转换
    int colorValue = color; // 需要显式转换static_cast<int>(color)

    // 4.可自定义底层类型, 控制内存布局
    enum class Color8Bit : short {
        RED,
        GREEN,
        BLUE,
        ORANGE // 橙色
    };

    d2x::check_eq(sizeof(Color), sizeof(int), "sizeof(Color) == sizeof(int)"); // 默认类型是int
    d2x::check_eq(sizeof(Color8Bit), sizeof(std::int8_t), "sizeof(Color8Bit) == sizeof(std::int8_t)"); // 可自定义类型int8_t

    // 5.自定义起始值: 默认情况下, 范围枚举类型的值从0开始, 往下递增
    enum class ErrorCode : int {
        OK = 0,
        ERROR_1,
        ERROR_2 = -2,
        ERROR_3
    };

    d2x::check_eq(static_cast<int>(ErrorCode::ERROR_3), 3, "static_cast<int>(ErrorCode::ERROR_3) == 3");

    d2x::wait();
    return 0;
}