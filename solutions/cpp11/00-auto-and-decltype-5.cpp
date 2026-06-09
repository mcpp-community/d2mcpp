// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/00-auto-and-decltype-5.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/00-auto-and-decltype-5.cpp
//

#include <d2x/cpp/common.hpp>

#include <type_traits>


// 5. const 与引用的剥离和保留

int main() {
    const int ci = 1;
    int n = 2;
    int& ri = n;

    bool type_check = false;

    // auto 剥离顶层 const: a 推导成 int
    auto a = ci;
    type_check = std::is_same<decltype(a), int>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // auto 剥离引用: b 推导成 int (b 是 n 的独立副本)
    auto b = ri;
    type_check = std::is_same<decltype(b), int>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // 用 const auto& 保留 const 引用 const int&
    const auto& cr = ci;
    type_check = std::is_same<decltype(cr), const int &>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // 用 decltype 精确保留声明类型 const int
    decltype(ci) d = ci;
    type_check = std::is_same<decltype(d), const int>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    return 0;
}
