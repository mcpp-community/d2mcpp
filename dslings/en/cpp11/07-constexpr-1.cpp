// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/07-constexpr-1.cpp
//
// Exercise: cpp11 | 07 - constexpr | Compile-time computation application examples
//
// Tips: Fix compiler errors based on compiler output
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/constexpr
//
// Auto-Checker command:
//
//   d2x checker constexpr
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

double pow(double base, int exp) {
    return exp == 0 ? 1.0 : base * pow(base, exp - 1);
}

constexpr double mysin(double x) {
    //constexpr double PI = 3.14159265358979323846;
    //constexpr double radius = x * PI / 180.0;
    #define radius(x) (x * 3.14159265358979323846 / 180.0)
    // (-1)^n * radius(x)^2n+1 / factorial(2n + 1);
    return radius(x)
        - pow(radius(x), 3) / factorial(3)
        + pow(radius(x), 5) / factorial(5);
}

int main() {

    // 1. Compile-time function computation
    constexpr int fact_10 = factorial(10);
    std::cout << "1 * 2 * .. * 10 = " << fact_10 << std::endl;

    // 2. Compile-time template parameter computation
    constexpr int sum_4 = Sum<4>::value;
    std::cout << "1 + 2 + 3 + 4 = " << sum_4 << std::endl;

    // 3. Compile-time computation example:
    //     What value makes value! + (1 + 2 + .. + value) > 10000?
    constexpr int value = 5;
    int f = factorial(value);
    constexpr int s = Sum<value>::value;
    constexpr int ans = f + s;

    static_assert(ans > 10000);

    // 4. Compile-time sin value computation (automatic lookup table) - Time complexity O(1)
    constexpr double sin30 = mysin(30.0);
    std::cout << "mysin(30): " << sin30 << " " << std::endl;

    D2X_WAIT

    return 0;
}