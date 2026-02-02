// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/14-type-alias-0.cpp
//
// Exercise/练习: cpp11 | 14 - type alias | 基本类型别名
//
// Tips/提示: 使用using关键字定义类型别名，替换传统的typedef
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/type_alias
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/14-type-alias.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker type-alias
//

#include <d2x/cpp/common.hpp>

#include <type_traits>

int main() {

    // 1. 基本类型别名定义
    D2X_YOUR_ANSWER Integer = int;
    D2X_YOUR_ANSWER = int;

    bool ok = std::is_same<Integer, int>::value; d2x_assert(ok);
    ok = std::is_same<Real, double>::value; d2x_assert(ok);

    // 2. 使用类型别名
    Integer a = 42;
    Real b = 3.14;

    // 3. 验证类型别名
    d2x_assert_eq(a, 42);
    d2x_assert_eq(b, 3.14);

    // 4. 类型别名本质相同
    int c = 100;
    Integer d = c;  // 可以赋值，因为本质都是int

    d2x_assert_eq(c, d);

    D2X_WAIT

    return 0;
}
