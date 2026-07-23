// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/12-nullptr/2.cpp
//
// Exercise: cpp11 | 12 - nullptr | Pointer Literal - Template Programming
//
// Tips: Use nullptr for better type safety in template programming
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/nullptr
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/cpp11/12-nullptr.md
//
// Exercise discussion: http://forum.d2learn.org/post/360
//
// Auto-Checker command:
//
//   d2x checker nullptr
//

#include <cstddef>   // NULL 宏是这一课的教具,import std 不带宏
import std;
import d2x;

// Template function example
template<typename T>
void processPointer(T* ptr) { }

// Template function that returns a copy of the input value
template<typename T>
T clone(const T& t) {
    return t;
}

int main() {

    // 1. Type deduction of nullptr in templates
    auto ptr1 = nullptr;  // ptr1's type is std::nullptr_t
    static_assert(std::is_same<decltype(ptr1), std::nullptr_t>::value, "Type deduction error");

    // 2. Advantages of nullptr in template functions - type deduction safety
    // Observe compiler errors, try to explicitly specify template parameters to resolve errors
    processPointer<int>(clone<int *>(0));
    processPointer<char>(clone(NULL));

    processPointer<int>(clone(nullptr));
    processPointer<char>(clone(nullptr));

    d2x::wait();
    return 0;
}