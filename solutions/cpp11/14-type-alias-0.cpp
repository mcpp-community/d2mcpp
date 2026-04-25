// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/14-type-alias-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/14-type-alias-0.cpp
//

#include <d2x/cpp/common.hpp>

#include <type_traits>

int main() {

    // 1. 基本类型别名定义
    using Integer = int;
    using Real = double;

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

    return 0;
}
