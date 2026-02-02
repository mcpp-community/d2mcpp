// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/06-scoped-enums-1.cpp
//
// Exercise: cpp11 | 06 - scoped enums | Scoped enum type basic usage
//
// Tips: Fix code through compiler error prompts and understand potential issues with traditional enum types
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/enum
//
// Auto-Checker command:
//
//   d2x checker scoped-enums-1
//

#include <d2x/cpp/common.hpp>

#include <iostream>

enum class Color {
    RED,
    GREEN,
    BLUE,
    ORANGE // Orange color
};

enum Fruit {
    Apple,
    Banana,
    ORANGE // Orange fruit
};

int main() {

    // 1. Scope qualification: Use scoped enum types to solve ORANGE type conflict problem
    Color color = Color::ORANGE;
    Fruit fruit = Fruit::ORANGE;

    d2x_assert(color == Color::ORANGE);
    d2x_assert(fruit == Fruit::ORANGE);

    // 2. Type safety: Prevent comparison between different enum type values
    if (color == Fruit::ORANGE) { // Use Color type to fix compilation error
        d2x_assert(color == Color::ORANGE);
    }

    // 3. Type checking: By default, scoped enum type values cannot be implicitly converted
    int colorValue = color; // Need explicit conversion static_cast<int>(color)

    // 4. Can customize underlying type to control memory layout
    enum class Color8Bit : short {
        RED,
        GREEN,
        BLUE,
        ORANGE // Orange color
    };

    d2x_assert_eq(sizeof(Color), sizeof(int)); // Default type is int
    d2x_assert_eq(sizeof(Color8Bit), sizeof(int8_t)); // Can customize type int8_t

    // 5. Custom starting value: By default, scoped enum type values start from 0 and increment downward
    enum class ErrorCode : int {
        OK = 0,
        ERROR_1,
        ERROR_2 = -2,
        ERROR_3
    };

    d2x_assert_eq(static_cast<int>(ErrorCode::ERROR_3), 3);

    D2X_WAIT

    return 0;
}