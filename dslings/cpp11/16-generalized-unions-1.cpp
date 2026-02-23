// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/16-generalized-unions-1.cpp
//
// Exercise/练习: cpp11 | 16 - generalized unions | 广义非平凡联合体
//
// Tips/提示:
// - 允许联合体包含非平凡类型(非 POD)
//
// Docs/文档:
//   - https://cppreference.com/w/cpp/language/union.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/16-generalized-unions.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker generalized-unions
//

#include <d2x/cpp/common.hpp>
#include <vector>

union M
{
    int a1;
    D2X_YOUR_ANSWER a2;
    M() {}
    M(const std::vector<int>& vec) : a2(vec) {
    }
    ~M() {}
};


int main() {

    M u1;
    u1.a1 = 21;

    // 1. 使用placement new构造
    new (&u1.a2) std::vector<int>();

    u1.a2 = {1, D2X_YOUR_ANSWER, 3};

    // 2. 验证：正常访问
    d2x_assert_eq(u1.a2[1], 42);

    // 3. 手动析构
    u1.a2.~vector();

    D2X_WAIT

    return 0;
}
