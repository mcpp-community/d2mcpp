// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-5.cpp
//
// Exercise/练习: cpp11 | 00 - auto and decltype | const 与引用的剥离和保留
//
// Tips/提示: auto 会剥离顶层 const 和引用, 用 const auto& / auto& 保留, decltype 精确保留
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
//   d2x checker auto-and-decltype-5
//

#include <d2x/cpp/common.hpp>

#include <type_traits>


// 5. const 与引用的剥离和保留

int main() {
    const int ci = 1;
    int n = 2;
    int& ri = n;

    bool type_check = false;

    // auto 剥离顶层 const: a 推导成什么?
    auto a = ci;
    type_check = std::is_same<decltype(a), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // auto 剥离引用: b 推导成什么?(b 是 n 的独立副本)
    auto b = ri;
    type_check = std::is_same<decltype(b), D2X_YOUR_ANSWER>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // 想保留 const 引用 const int&: cr 该怎么声明?
    D2X_YOUR_ANSWER cr = ci;
    type_check = std::is_same<decltype(cr), const int &>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // 想精确保留声明类型 const int: d 该怎么声明?
    D2X_YOUR_ANSWER d = ci;
    type_check = std::is_same<decltype(d), const int>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    D2X_WAIT

    return 0;
}
