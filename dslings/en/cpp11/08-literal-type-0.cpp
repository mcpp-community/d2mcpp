// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/08-literal-type-0.cpp
//
// Exercise: cpp11 | 08 - literal type | What are literal types
//
// Tips: Fix compiler errors based on compiler output, understand the concept of literal types
//
// Docs:
//   - https://en.cppreference.com/w/cpp/named_req/LiteralType.html
//
// Auto-Checker command:
//
//   d2x checker literal-type-0
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <array>
#include <string>

constexpr char compile_time_compute(char c, int a) {
    return a + c;
}

constexpr std::array<int, 3> to_array(const char *str /*, len = 3 */) {
    return {str[0] - '0', str[1] - '0', str[2] - '0'};
}

constexpr std::string to_string(const std::array<int, 3> &arr) {
    return std::string(1, '0' + arr[0]) +
           std::string(1, '0' + arr[1]) +
           std::string(1, '0' + arr[2]);
}

int main() {

    // 1. Literal types that can participate in compile-time computation
    constexpr char c = 'A';
    constexpr int a = 1;
    constexpr std::array<int, 3> arr = {1, 2, 3};
    constexpr std::string str = "123";

    constexpr auto result = compile_time_compute(c, a);
    std::cout << result << std::endl;

    // 2. "Complex" aggregate/pointer/reference literal types
    constexpr auto arr_to_str = to_string(arr);
    std::cout << arr_to_str.data() << std::endl;

    constexpr auto str_to_arr = to_array(str);
    constexpr int sum = str_to_arr[0] + str_to_arr[1] + str_to_arr[2];

    std::cout << "1 + 2 + 3 = " <<  sum << std::endl;

    D2X_WAIT

    return 0;
}