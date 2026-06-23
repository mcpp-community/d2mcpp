// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/06-digit-separators-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/06-digit-separators-0.cpp
//

#include <d2x/cpp/common.hpp>

int main() {

    int million = 1'000'000;
    d2x_assert_eq(million, 1000000);

    int hex_color = 0xFF'AA'BB;
    d2x_assert_eq(hex_color, 0xFFAABB);

    int bin_val = 0b1010'1100;
    d2x_assert_eq(bin_val, 0b10101100);

    return 0;
}
