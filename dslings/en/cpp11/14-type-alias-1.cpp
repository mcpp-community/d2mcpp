// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp11/14-type-alias-1.cpp
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

#include <d2x/common.hpp>

#include <vector>
#include <string>

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
    d2x_assert_eq(func_called, 3);

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
    d2x_assert(strings[0] == "hello");
    d2x_assert(strings[1] == "world");
    d2x_assert_eq(value, 100);

    D2X_WAIT

    return 0;
}