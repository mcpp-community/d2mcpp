// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/05-binary-literals-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/05-binary-literals-0.cpp
//

#include <d2x/cpp/common.hpp>

constexpr unsigned READ  = 0b001;
constexpr unsigned WRITE = 0b010;
constexpr unsigned EXEC  = 0b100;

bool has_perm(unsigned perm, unsigned flag) { return perm & flag; }

int main() {

    unsigned p1 = 0b101;
    d2x_assert(has_perm(p1, READ));
    d2x_assert(!has_perm(p1, WRITE));
    d2x_assert(has_perm(p1, EXEC));

    unsigned p2 = 0b111;
    d2x_assert(has_perm(p2, READ));
    d2x_assert(has_perm(p2, WRITE));
    d2x_assert(has_perm(p2, EXEC));

    d2x_assert_eq(0b1111, 15);

    return 0;
}
