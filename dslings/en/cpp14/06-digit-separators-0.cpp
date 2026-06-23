// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp14/06-digit-separators-0.cpp
//
// Exercise: cpp14 | 06 - digit separators
//
// Tips:
// - Single quotes ' separate digits without affecting the value
// - Works with decimal, hex, and binary literals
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/integer_literal
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/06-digit-separators.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
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
