// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/cpp11/tests/13-long-long/1.cpp
//
// Exercise/练习: cpp11 | 13 - long long | 64位整数类型 - 大数应用和边界值
//
// Tips/提示: 使用 long long 处理大整数和边界值计算
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

import std;
import d2x;


int main() {

    // 1. 获取整数类型的边界值
    auto maxInt = std::numeric_limits<long long>::max();
    auto maxLL = std::numeric_limits<int>::max();
    auto minLL = std::numeric_limits<unsigned long long>::min();
    auto maxULL = std::numeric_limits<unsigned int>::max();

    d2x::check_eq(maxInt, 2147483647, "maxInt == 2147483647");
    d2x::check_eq(maxLL, 9223372036854775807LL, "maxLL == 9223372036854775807LL");
    d2x::check_eq(minLL, -9223372036854775807LL - 1, "minLL == -9223372036854775807LL - 1");
    d2x::check_eq(maxULL, 18446744073709551615ULL, "maxULL == 18446744073709551615ULL");

    // 2. 大整数应用 - 表示世界人口
    int currentPopulation = 7800000000;
    d2x::check_eq(currentPopulation, 7800000000ULL, "currentPopulation == 7800000000ULL");

    d2x::wait();
    return 0;
}
