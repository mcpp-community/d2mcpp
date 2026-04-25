// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/00-auto-and-decltype-4.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/00-auto-and-decltype-4.cpp
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
    type_check = std::is_same<decltype((obj)), const Object &>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // obj.a的类型推导 和 (obj.a) 的类型推导
    type_check = std::is_same<decltype(obj.a), const int>::value;
    d2x_assert(type_check); type_check = false; // dont change this line
    type_check = std::is_same<decltype((obj.a)), const int &>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    // obj.b的类型推导 和 (obj.b) 的类型推导
    type_check = std::is_same<decltype(obj.b), double>::value;
    d2x_assert(type_check); type_check = false; // dont change this line
    type_check = std::is_same<decltype((obj.b)), const double &>::value;
    d2x_assert(type_check); type_check = false; // dont change this line

    return 0;
}
