// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/06-scoped-enums-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/06-scoped-enums-1.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>

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

    d2x_assert(color == Color::ORANGE);
    d2x_assert(fruit == Fruit::ORANGE);

    // 2.类型安全: 防止不同类型的枚举值之间的比较
    if (color == Color::ORANGE) { // 使用Color类型修复编译错误
        d2x_assert(color == Color::ORANGE);
    }

    // 3.类型检查: 默认情况下, 范围枚举类型的值是不可隐式转换
    int colorValue = static_cast<int>(color);
    (void)colorValue;

    // 4.可自定义底层类型, 控制内存布局
    enum class Color8Bit : int8_t {
        RED,
        GREEN,
        BLUE,
        ORANGE // 橙色
    };

    d2x_assert_eq(sizeof(Color), sizeof(int)); // 默认类型是int
    d2x_assert_eq(sizeof(Color8Bit), sizeof(int8_t)); // 可自定义类型int8_t

    // 5.自定义起始值: 默认情况下, 范围枚举类型的值从0开始, 往下递增
    enum class ErrorCode : int {
        OK = 0,
        ERROR_1,
        ERROR_2 = -2,
        ERROR_3 = 3
    };

    d2x_assert_eq(static_cast<int>(ErrorCode::ERROR_3), 3);

    return 0;
}
