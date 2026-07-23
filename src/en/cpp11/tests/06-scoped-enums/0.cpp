// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/06-scoped-enums/0.cpp
//
// Exercise: cpp11 | 06 - scoped enums | Traditional enum type potential issues
//
// Tips: Fix code through compiler error prompts and understand potential issues with traditional enum types
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/enum
//
// Auto-Checker command:
//
//   d2x checker scoped-enums
//

import std;
import d2x;


enum Color {
    RED,
    GREEN,
    BLUE,
    ORANGE // 1. Type conflict - Orange color
};

enum Fruit {
    Apple,
    Banana,
    ORANGE // 1. Type conflict - Orange fruit
};

int main() {
 
    Color color = RED;
    Fruit fruit = Apple;

    d2x::check_eq(color, RED, "color == RED");
    d2x::check_eq(fruit, Apple, "fruit == Apple");

    // 2. Syntactically correct, but logically wrong type matching
    if (color == Apple) { // Do not delete this line of code
        // Code will run here
        d2x::wait();
    }

    if (fruit == RED) {
        d2x::wait();
    }

    d2x::wait();
    return 0;
}