// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/03-trailing-return-type.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/03-trailing-return-type.cpp
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
auto add2(const T1 &a, const T2 &b) -> decltype(a + b) {
    return a + b;
}

auto add3 = [](double a, double b) -> int {
    return a + b;
};

int main() {

    d2x_assert_eq(add0(1.1, 2), 3);
    d2x_assert_eq(add1(1.1, 2), 3);
    d2x_assert_eq(add2(1.1, 2), 3.1);
    d2x_assert_eq(add2(1, 2.1), 3.1);
    d2x_assert_eq(add3(1.1, 2.1), 3);

    return 0;
}
