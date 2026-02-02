// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-4.cpp
//
// Exercise: cpp11 | 00 - auto and decltype | Class/Struct Member Type Deduction
//
// Tips: Fill in the correct deduced type at D2X_YOUR_ANSWER
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
//   d2x checker auto-and-decltype-4
//

#include <d2x/cpp/common.hpp>

#include <type_traits>


// 4. Class/Struct member type deduction

struct Object {
    const int a;
    double b;
    Object() : a(1), b(2.0) { }
};

int main() {
    const Object obj;

    bool type_check = false;

    // Type deduction for obj and (obj)
    type_check = std::is_same<decltype(obj), const Object>::value;
    d2x_assert(type_check); type_check = false; // dont change this line
    type_check = std::is_same<decltype((obj)), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // Type deduction for obj.a and (obj.a)
    type_check = std::is_same<decltype(obj.a), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line
    type_check = std::is_same<decltype((obj.a)), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // Type deduction for obj.b and (obj.b)
    type_check = std::is_same<decltype(obj.b), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line
    type_check = std::is_same<decltype((obj.b)), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    D2X_WAIT

    return 0;
}
