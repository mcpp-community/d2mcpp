// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp11/13-long-long-0.cpp
//
// Exercise: cpp11 | 13 - long long | 64-bit Integer Type - Basic Usage
//
// Tips: Use long long type and corresponding literal suffixes
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/types
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/13-long-long.md
//
// Exercise discussion: http://forum.d2learn.org
//
// Auto-Checker command:
//
//   d2x checker long-long
//

#include <d2x/cpp/common.hpp>

#include <type_traits>

int main() {

    // 1. Basic declaration and initialization
    D2X_YOUR_ANSWER val1 = 1;
    D2X_YOUR_ANSWER val2 = -1;

    // 2. Integer representation range
    unsigned int uVal1 = 18446744073709551615;
    d2x_assert_eq(uVal1, 18446744073709551615ULL);

    // 3. Type deduction and literals
    // Fix the declarations below to let auto correctly deduce the types
    auto longlong = 1234567890;
    auto ulonglong = 9876543210;

    bool is_longlong = std::is_same<decltype(longlong), long long>::value;
    bool is_ulonglong = std::is_same<decltype(ulonglong), unsigned long long>::value;

    d2x_assert(is_longlong == true);
    d2x_assert(is_ulonglong == true);

    D2X_WAIT

    return 0;
}
