// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/07-constexpr-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/07-constexpr-1.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>

template <int N>
struct Sum {
    static constexpr int value = Sum<N - 1>::value + N;
};

template <>
struct Sum<1> { static constexpr int value = 1; };

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr double pow(double base, int exp) {
    return exp == 0 ? 1.0 : base * pow(base, exp - 1);
}

constexpr double mysin(double x) {
    #define radius(x) (x * 3.14159265358979323846 / 180.0)
    return radius(x)
        - pow(radius(x), 3) / factorial(3)
        + pow(radius(x), 5) / factorial(5);
}

int main() {

    // 1. 编译期-函数计算
    constexpr int fact_10 = factorial(10);
    std::cout << "1 * 2 * .. * 10 = " << fact_10 << std::endl;

    // 2. 编译期-模板参数计算
    constexpr int sum_4 = Sum<4>::value;
    std::cout << "1 + 2 + 3 + 4 = " << sum_4 << std::endl;

    // 3. 编译期计算示例:
    //     value是多少时? value! + (1 + 2 + .. + value) > 10000
    //   value=7: 5040 + 28 = 5068 (不够)
    //   value=8: 40320 + 36 = 40356 (满足)
    constexpr int value = 8;
    constexpr int f = factorial(value);
    constexpr int s = Sum<value>::value;
    constexpr int ans = f + s;

    static_assert(ans > 10000, "ans should be > 10000");

    // 4. 编译期计算sin值(自动打表) - 时间复杂度O(1)
    constexpr double sin30 = mysin(30.0);
    std::cout << "mysin(30): " << sin30 << " " << std::endl;

    return 0;
}
