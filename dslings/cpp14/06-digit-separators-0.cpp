// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp14/06-digit-separators-0.cpp
//
// Exercise/练习: cpp14 | 06 - digit separators | 数字分隔符
//
// Tips/提示:
// - 单引号 ' 分隔数字, 不影响数值
// - 可用于十进制、十六进制、二进制字面量
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/integer_literal
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/06-digit-separators.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker digit-separators
//

#include <d2x/cpp/common.hpp>

int main() {

    int million = 1'D2X_YOUR_ANSWER'000;
    d2x_assert_eq(million, 1000000);

    int hex_color = 0xFF'D2X_YOUR_ANSWER'BB;
    d2x_assert_eq(hex_color, 0xFFAABB);

    int bin_val = 0b1010'D2X_YOUR_ANSWER;
    d2x_assert_eq(bin_val, 0b10101100);

    D2X_WAIT

    return 0;
}
