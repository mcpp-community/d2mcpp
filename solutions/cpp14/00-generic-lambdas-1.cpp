// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/00-generic-lambdas-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/00-generic-lambdas-1.cpp
//

#include <d2x/cpp/common.hpp>
#include <vector>
#include <algorithm>

int main() {

    // 0. 同一个泛型 lambda 用于不同元素类型的容器排序
    std::vector<int> v1 = {5, 1, 4, 2, 8};
    std::vector<double> v2 = {3.1, 2.7, 8.5, 1.9};

    auto desc = [](auto a, auto b) {
        return a > b;
    };

    std::sort(v1.begin(), v1.end(), desc);
    d2x_assert_eq(v1[0], 8);
    d2x_assert_eq(v1[4], 1);

    std::sort(v2.begin(), v2.end(), desc);
    d2x_assert_eq(v2[0], 8.5);

    // 1. 带捕获的泛型 lambda — find_if
    int threshold = 3;
    auto above = [threshold](auto x) {
        return x > threshold;
    };

    auto it1 = std::find_if(v1.begin(), v1.end(), above);
    d2x_assert(*it1 == 8);

    auto it2 = std::find_if(v2.begin(), v2.end(), above);
    d2x_assert(*it2 == 8.5);

    // 2. 泛型 lambda 返回 lambda — 函数工厂
    auto make_multiplier = [](auto factor) {
        return [factor](auto x) { return x * factor; };
    };

    auto times2 = make_multiplier(2);
    d2x_assert_eq(times2(10), 20);
    d2x_assert_eq(times2(0.5), 1.0);

    return 0;
}
