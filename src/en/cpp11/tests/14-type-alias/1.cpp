// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/14-type-alias/1.cpp
//
// Exercise: cpp11 | 14 - type alias | Complex Types and Function Pointer Aliases
//
// Tips: Use the using keyword to define aliases for complex types and function pointers
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/type_alias
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/14-type-alias.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker Command:
//
//   d2x checker type-alias
//

import std;
import d2x;


static int func_called = 0;

// Function declaration
void example_func(int a, int b) {
    func_called = a + b;
}

int main() {

    // 1. Function pointer alias
    D2X_YOUR_ANSWER FuncPtr = void(*)(int, int);

    FuncPtr func = example_func;

    func(1, 2);
    d2x::check_eq(func_called, 3, "func_called == 3");

    // 2. Container type alias
    // Use using to define an alias for vector<string>
    D2X_YOUR_ANSWER = std::vector<std::string>;

    // Using container type alias
    StringVector strings = {"hello", "world"};

    // 3. Nested type alias
    struct Container {
        ValueType = int
    };

    Container::ValueType value = 100;

    // 4. Verifying type aliases
    d2x::check(strings[0] == "hello", "strings[0] == \"hello\"");
    d2x::check(strings[1] == "world", "strings[1] == \"world\"");
    d2x::check_eq(value, 100, "value == 100");

    d2x::wait();
    return 0;
}