// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/15-variadic-templates-0.cpp
//
// Exercise: cpp11 | 15 - variadic templates | Variadic templates basics
//
// Tips:
// - Variadic templates allow a function to accept any number of arguments
// - In C++11, parameter packs are typically processed via recursion
// - A recursion terminating function (base case) must be defined
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/parameter_pack
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/15-variadic-templates.md
//
// Practice discussion: http://forum.d2learn.org/category/20
//
// Auto-Checker command:
//
//   d2x checker variadic-templates
//

#include <d2x/cpp/common.hpp>
#include <sstream>

std::stringstream ss;

// Define the recursion terminating function
// This is invoked when the parameter pack is empty
D2X_YOUR_ANSWER

// Define the variadic template function
template<typename T,D2X_YOUR_ANSWER>
void print(T first, D2X_YOUR_ANSWER args) {
    ss << first << " ";
    // Recursive call: process the remaining arguments
    print(D2X_YOUR_ANSWER);
}

int main() {
    print(1, "hello", 3.14);

    std::string result = ss.str();
    d2x_assert(result == "1 hello 3.14 ");

    D2X_WAIT

    return 0;
}
