// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/13-long-long-0.cpp
//
// Exercise/练习: cpp11 | 13 - long long | 64位整数类型 - 基础用法
//
// Tips/提示: 使用 long long 类型和对应的字面量后缀
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/types
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/13-long-long.md
//
// 练习交流讨论: http://forum.d2learn.org
//
// Auto-Checker/自动检测命令:
//
//   d2x checker long-long
//

#include <d2x/common.hpp>

#include <type_traits>

int main() {

    // 1. 基本声明和初始化
    D2X_YOUR_ANSWER val1 = 1;
    D2X_YOUR_ANSWER val2 = -1;

    // 2. 整数表示范围
    unsigned int uVal1 = 18446744073709551615;
    d2x_assert_eq(uVal1, 18446744073709551615ULL);

    // 3. 类型推导和字面量
    // 修复下面的声明，让 auto 正确推导类型
    auto longlong = 1234567890;
    auto ulonglong = 9876543210;

    bool is_longlong = std::is_same<decltype(longlong), long long>::value;
    bool is_ulonglong = std::is_same<decltype(ulonglong), unsigned long long>::value;

    d2x_assert(is_longlong == true);
    d2x_assert(is_ulonglong == true);

    D2X_WAIT

    return 0;
}
