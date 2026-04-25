// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/00-auto-and-decltype-3.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/00-auto-and-decltype-3.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <vector>

// 3. 函数返回值类型

auto add_func(int a, double b) -> decltype(a + b) {
    return a + b;
}

template<typename T1, typename T2>
auto minus_func(T1 a, T2 b) -> decltype(a - b) {
    return a - b;
}

int main() {

    d2x_assert_eq(minus_func(1, 2), -1);
    d2x_assert_eq(minus_func(2, 1), 1);
    d2x_assert_eq(minus_func(1, 2.1), -1.1);

    return 0;
}
