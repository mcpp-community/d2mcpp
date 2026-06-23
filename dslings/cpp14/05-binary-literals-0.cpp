// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp14/05-binary-literals-0.cpp
//
// Exercise/练习: cpp14 | 05 - binary literals | 二进制字面量位掩码
//
// Tips/提示:
// - 0b / 0B 前缀表示二进制字面量
// - 位掩码用二进制表示最直观
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/integer_literal
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/05-binary-literals.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker binary-literals
//

#include <d2x/cpp/common.hpp>

constexpr unsigned READ  = 0b001;
constexpr unsigned WRITE = D2X_YOUR_ANSWER;
constexpr unsigned EXEC  = 0b100;

bool has_perm(unsigned perm, unsigned flag) { return perm & flag; }

int main() {

    unsigned p1 = 0b101;
    d2x_assert(has_perm(p1, READ));
    d2x_assert(!has_perm(p1, WRITE));
    d2x_assert(has_perm(p1, EXEC));

    unsigned p2 = D2X_YOUR_ANSWER;
    d2x_assert(has_perm(p2, READ));
    d2x_assert(has_perm(p2, WRITE));
    d2x_assert(has_perm(p2, EXEC));

    d2x_assert_eq(0b1111, D2X_YOUR_ANSWER);

    D2X_WAIT

    return 0;
}
