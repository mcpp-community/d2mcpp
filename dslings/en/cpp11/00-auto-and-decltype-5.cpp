// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-5.cpp
//
// Exercise: cpp11 | 00 - auto and decltype | const & reference stripping and preservation
//
// Tips: auto strips top-level const and references; use const auto& / auto& to keep them, decltype preserves exactly
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/auto
//   - https://en.cppreference.com/w/cpp/language/decltype
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/00-auto-and-decltype.md
//
// Forum: https://forum.d2learn.org/category/20
//
// Auto-Checker command:
//
//   d2x checker auto-and-decltype-5
//

#include <d2x/cpp/common.hpp>

#include <type_traits>


// 5. const & reference stripping and preservation

int main() {
    const int ci = 1;
    int n = 2;
    int& ri = n;

    bool type_check = false;

    // auto strips top-level const: what does a deduce to?
    auto a = ci;
    type_check = std::is_same<decltype(a), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // auto strips the reference: what does b deduce to? (b is an independent copy of n)
    auto b = ri;
    type_check = std::is_same<decltype(b), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // To keep a const reference const int&: how should cr be declared?
    D2X_YOUR_ANSWER cr = ci;
    type_check = std::is_same<decltype(cr), const int &>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // To preserve the declared type const int exactly: how should d be declared?
    D2X_YOUR_ANSWER d = ci;
    type_check = std::is_same<decltype(d), const int>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    D2X_WAIT

    return 0;
}
