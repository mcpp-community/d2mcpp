// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/13-long-long-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/13-long-long-0.cpp
//

#include <d2x/cpp/common.hpp>

#include <type_traits>

int main() {

    // 1. 基本声明和初始化
    long long val1 = 1;
    long long val2 = -1;
    (void)val1; (void)val2;

    // 2. 整数表示范围（unsigned int 装不下，需要 unsigned long long）
    unsigned long long uVal1 = 18446744073709551615ULL;
    d2x_assert_eq(uVal1, 18446744073709551615ULL);

    // 3. 类型推导和字面量后缀
    auto longlong = 1234567890LL;
    auto ulonglong = 9876543210ULL;

    bool is_longlong = std::is_same<decltype(longlong), long long>::value;
    bool is_ulonglong = std::is_same<decltype(ulonglong), unsigned long long>::value;

    d2x_assert(is_longlong == true);
    d2x_assert(is_ulonglong == true);

    return 0;
}
