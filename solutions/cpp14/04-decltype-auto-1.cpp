// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/04-decltype-auto-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/04-decltype-auto-1.cpp
//

#include <d2x/cpp/common.hpp>

int val = 50;

decltype(auto) return_name() {
    return val;
}

decltype(auto) return_paren() {
    return (val);
}

int main() {

    decltype(auto) v1 = val;
    v1 = 99;
    d2x_assert_eq(val, 50);

    decltype(auto) v2 = (val);
    v2 = 99;
    d2x_assert_eq(val, 99);

    auto a1 = return_name();
    d2x_assert_eq(a1, 99);
    ++a1;
    d2x_assert_eq(val, 99);

    auto& a2 = return_paren();
    d2x_assert_eq(a2, 99);
    ++a2;
    d2x_assert_eq(val, 100);

    return 0;
}
