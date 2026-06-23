// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/04-decltype-auto-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/04-decltype-auto-0.cpp
//

#include <d2x/cpp/common.hpp>

int g_val = 100;

auto get_ref_auto() {
    return (g_val);
}

decltype(auto) get_ref_declauto() {
    return (g_val);
}

template <typename T>
decltype(auto) forward_value(T&& arg) {
    return std::forward<T>(arg);
}

int main() {

    auto a1 = get_ref_auto();
    d2x_assert_eq(a1, 100);
    ++a1;
    d2x_assert_eq(g_val, 100);

    auto& a2 = get_ref_declauto();
    d2x_assert_eq(a2, 100);
    ++a2;
    d2x_assert_eq(g_val, 101);

    int x = 10;
    auto& f1 = forward_value<int&>(x);
    d2x_assert_eq(f1, 10);
    ++f1;
    d2x_assert_eq(x, 11);

    return 0;
}
