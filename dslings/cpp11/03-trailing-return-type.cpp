// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/03-trailing-return-type.cpp
//
// Exercise/练习: cpp11 | 03 - trailing return type
//
// Tips/提示: 替换D2X_YOUR_ANSWER为正确的类型, 通过所有检查
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/function#Function_declaration
//
// Auto-Checker/自动检测命令:
//
//   d2x checker trailing-return-type
//

#include <d2x/cpp/common.hpp>

#include <iostream>

int add0(double a, int b) {
    return a + b;
}

auto add1(double a, int b) -> int {
    return a + b;
}

template<typename T1, typename T2>
auto add2(const T1 &a, const T2 &b) -> D2X_YOUR_ANSWER {
    return a + b;
}

auto add3 = [](double a, double b) -> D2X_YOUR_ANSWER {
    return a + b;
};

int main() {

    d2x_assert_eq(add0(1.1, 2), 3);
    d2x_assert_eq(add1(1.1, 2), 3);
    d2x_assert_eq(add2(1.1, 2), 3.1);
    d2x_assert_eq(add2(1, 2.1), 3.1);
    d2x_assert_eq(add3(1.1, 2.1), 3);

    D2X_WAIT

    return 0;
}