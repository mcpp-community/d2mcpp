// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp14/04-decltype-auto-1.cpp
//
// Exercise/练习: cpp14 | 04 - decltype(auto) | 括号陷阱与变量声明
//
// Tips/提示:
// - decltype(auto) 在变量声明中遵循 decltype 推导规则
// - return x 和 return (x) 在 decltype(auto) 下推导结果不同
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

int val = 50;

// 返回名 → 值类型
decltype(auto) return_name() {
    return D2X_YOUR_ANSWER;
}

// 返回 (名) → 引用类型
decltype(auto) return_paren() {
    return (D2X_YOUR_ANSWER);
}

int main() {

    // 0. 变量声明中使用 decltype(auto)
    D2X_YOUR_ANSWER v1 = val;
    v1 = 99;
    d2x_assert_eq(val, 50);

    decltype(auto) v2 = (D2X_YOUR_ANSWER);
    v2 = 99;
    d2x_assert_eq(val, 99);

    // 1. return_name 返回的是值, 修改不影响原变量
    auto a1 = return_name();
    d2x_assert_eq(a1, 50);
    ++a1;
    d2x_assert_eq(val, 99);

    // 2. return_paren 返回的是引用, 修改直接影响原变量
    auto& a2 = return_paren();
    d2x_assert_eq(a2, D2X_YOUR_ANSWER);
    ++a2;
    d2x_assert_eq(val, 100);

    D2X_WAIT

    return 0;
}
