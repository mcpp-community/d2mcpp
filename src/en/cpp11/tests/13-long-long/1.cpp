// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/13-long-long/1.cpp
//
// Exercise: cpp11 | 13 - long long | 64-bit Integer Type - Large Number Applications and Boundary Values
//
// Tips: Use long long to handle large integers and boundary value calculations
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/types
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/13-long-long.md
//
// Exercise discussion: http://forum.d2learn.org
//
// Auto-Checker command:
//
//   d2x checker long-long
//

import std;
import d2x;


int main() {

    // 1. Get boundary values of integer types
    auto maxInt = std::numeric_limits<long long>::max();
    auto maxLL = std::numeric_limits<int>::max();
    auto minLL = std::numeric_limits<unsigned long long>::min();
    auto maxULL = std::numeric_limits<unsigned int>::max();

    d2x::check_eq(maxInt, 2147483647, "maxInt == 2147483647");
    d2x::check_eq(maxLL, 9223372036854775807LL, "maxLL == 9223372036854775807LL");
    d2x::check_eq(minLL, -9223372036854775807LL - 1, "minLL == -9223372036854775807LL - 1");
    d2x::check_eq(maxULL, 18446744073709551615ULL, "maxULL == 18446744073709551615ULL");

    // 2. Large integer application - representing world population
    int currentPopulation = 7800000000;
    d2x::check_eq(currentPopulation, 7800000000ULL, "currentPopulation == 7800000000ULL");

    d2x::wait();
    return 0;
}