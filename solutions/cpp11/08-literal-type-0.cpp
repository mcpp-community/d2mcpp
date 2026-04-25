// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/08-literal-type-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/08-literal-type-0.cpp
//
// 教学要点: std::string 在 C++17 下并不是字面值类型, 不能 constexpr,
// 这里把涉及 std::string 的部分降级为运行时, 其余字面值类型 (char/int/std::array)
// 仍可参与编译期计算。

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

// std::string 在 C++17 下不是字面值类型, 这里改为运行时函数
std::string to_string(const std::array<int, 3> &arr) {
    return std::string(1, '0' + arr[0]) +
           std::string(1, '0' + arr[1]) +
           std::string(1, '0' + arr[2]);
}

int main() {

    // 1.能参与编译期计算的字面值类型
    constexpr char c = 'A';
    constexpr int a = 1;
    constexpr std::array<int, 3> arr = {1, 2, 3};
    std::string str = "123"; // std::string 不是字面值类型, 改为运行时变量

    constexpr auto result = compile_time_compute(c, a);
    std::cout << result << std::endl;

    // 2."复杂" 聚合/指针/引用等 字面值类型
    auto arr_to_str = to_string(arr); // 运行时调用
    std::cout << arr_to_str.data() << std::endl;

    constexpr auto str_to_arr = to_array("123"); // 仍可编译期, 直接使用字面量
    constexpr int sum = str_to_arr[0] + str_to_arr[1] + str_to_arr[2];

    std::cout << "1 + 2 + 3 = " <<  sum << std::endl;
    (void)str;

    return 0;
}
