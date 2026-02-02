// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-4.cpp
//
// Exercise/练习: cpp11 | 00 - auto and decltype | 类/结构体成员类型推导
//
// Tips/提示: 在D2X_YOUR_ANSWER处填写正确的推导类型
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/auto
//   - https://en.cppreference.com/w/cpp/language/decltype
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/00-auto-and-decltype.md
//
// 练习交流讨论: http://forum.d2learn.org/post/357
//
// Auto-Checker/自动检测命令:
//
//   d2x checker auto-and-decltype-4
//

#include <d2x/cpp/common.hpp>

#include <type_traits>


// 4. 类/结构体成员类型推导

struct Object {
    const int a;
    double b;
    Object() : a(1), b(2.0) { }
};

int main() {
    const Object obj;

    bool type_check = false;

    // obj的类型推导 和 (obj) 的类型推导
    type_check = std::is_same<decltype(obj), const Object>::value;
    d2x_assert(type_check); type_check = false; // dont change this line
    type_check = std::is_same<decltype((obj)), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // obj.a的类型推导 和 (obj.a) 的类型推导
    type_check = std::is_same<decltype(obj.a), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line
    type_check = std::is_same<decltype((obj.a)), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // obj.b的类型推导 和 (obj.b) 的类型推导
    type_check = std::is_same<decltype(obj.b), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line
    type_check = std::is_same<decltype((obj.b)), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    D2X_WAIT

    return 0;
}