// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/13-long-long-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/13-long-long-1.cpp
//

#include <d2x/cpp/common.hpp>

#include <limits>

int main() {

    // 1. 获取整数类型的边界值（每个变量的类型与名字呼应）
    auto maxInt = std::numeric_limits<int>::max();
    auto maxLL = std::numeric_limits<long long>::max();
    auto minLL = std::numeric_limits<long long>::min();
    auto maxULL = std::numeric_limits<unsigned long long>::max();

    d2x_assert_eq(maxInt, 2147483647);
    d2x_assert_eq(maxLL, 9223372036854775807LL);
    d2x_assert_eq(minLL, -9223372036854775807LL - 1);
    d2x_assert_eq(maxULL, 18446744073709551615ULL);

    // 2. 大整数应用 - 表示世界人口（int 装不下 7.8e9）
    unsigned long long currentPopulation = 7800000000ULL;
    d2x_assert_eq(currentPopulation, 7800000000ULL);

    return 0;
}
