// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/12-nullptr/0.cpp
//
// Exercise: cpp11 | 12 - nullptr | Pointer Literal - Basic Usage
//
// Tips: Use nullptr to replace traditional NULL and 0 for pointer initialization
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/nullptr
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/cpp11/12-nullptr.md
//
// Exercise discussion: http://forum.d2learn.org/post/358
//
// Auto-Checker command:
//
//   d2x checker nullptr
//

#include <cstddef>   // NULL 宏是这一课的教具,import std 不带宏
import std;
import d2x;


int main() {

    // 1. Initialize pointers
    int* ptr1 = nullptr; // Recommended usage
    int* ptr2 = NULL;   // Fix here, add correct type
    int* ptr3 = 0;      // Not recommended traditional usage

    d2x::check(ptr1 == nullptr, "ptr1 == nullptr");
    d2x::check(ptr2 == nullptr, "ptr2 == nullptr");
    d2x::check(ptr3 == nullptr, "ptr3 == nullptr");

    // 2. Type of nullptr
    bool ok = std::is_same<decltype(nullptr), D2X_YOUR_ANSWER>::value;
    d2x::check(ok, "ok");

    // 3. Use nullptr for pointer comparison
    int value = 42;
    int* ptr4 = &value;

    if (ptr4 != nullptr) {
        *ptr4 = D2X_YOUR_ANSWER;
        d2x::check_eq(*ptr4, 2233, "*ptr4 == 2233");
    }

    // 4. Different types of pointers can all use nullptr
    double* dptr = nullptr;
    char* cptr = nullptr
    void* vptr = nullptr;

    d2x::check(dptr == nullptr, "dptr == nullptr");
    d2x::check(cptr == nullptr, "cptr == nullptr");
    d2x::check(vptr == nullptr, "vptr == nullptr");

    d2x::wait();
    return 0;
}