// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp14/07-deprecated-attribute-0.cpp
//
// Exercise/练习: cpp14 | 07 - deprecated attribute | [[deprecated]]
//
// Tips/提示:
// - [[deprecated("message")]] 标记废弃的函数/变量
// - 调用时编译器产生警告
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/attributes
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/07-deprecated-attribute.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker deprecated-attribute
//

#include <d2x/cpp/common.hpp>

[[deprecated("Use new_add")]]
D2X_YOUR_ANSWER old_add(int a, int b) { return a + b; }

int new_add(int a, int b) { return a + b; }

int main() {

    int r = old_add(10, 20);
    d2x_assert_eq(r, D2X_YOUR_ANSWER);

    D2X_WAIT

    return 0;
}
