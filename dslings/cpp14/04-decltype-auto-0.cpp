// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp14/04-decltype-auto-0.cpp
//
// Exercise/练习: cpp14 | 04 - decltype(auto) | 引用保留与转发
//
// Tips/提示:
// - decltype(auto) 按 decltype 规则推导, 精确保留引用和 cv 限定
// - auto 返回会剥离引用, decltype(auto) 不会
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/auto
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/04-decltype-auto.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker decltype-auto
//

#include <d2x/cpp/common.hpp>

int g_val = 100;

auto get_ref_auto() {
    return (g_val);
}

D2X_YOUR_ANSWER get_ref_declauto() {
    return (D2X_YOUR_ANSWER);
}

template <typename T>
D2X_YOUR_ANSWER forward_value(T&& arg) {
    return std::forward<T>(D2X_YOUR_ANSWER);
}

int main() {

    auto a1 = get_ref_auto();
    d2x_assert_eq(a1, 100);
    ++a1;
    d2x_assert_eq(g_val, 100);

    auto& a2 = get_ref_declauto();
    d2x_assert_eq(a2, 100);
    ++a2;
    d2x_assert_eq(g_val, D2X_YOUR_ANSWER);

    int x = 10;
    auto& f1 = forward_value<int&>(x);
    d2x_assert_eq(f1, 10);
    ++f1;
    d2x_assert_eq(x, 11);

    D2X_WAIT

    return 0;
}
