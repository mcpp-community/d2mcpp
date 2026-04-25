// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/00-auto-and-decltype-2.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/00-auto-and-decltype-2.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <vector>
#include <functional>

int add_func(int a, int b) {
    return a + b;
}

int main() {

    // 2. 复杂类型

    std::vector<int> v = {1, 2, 3};

    std::vector<int>::iterator v1 = v.begin();
    for (; v1 != v.end(); ++v1) {
        std::cout << *v1 << " ";
    }
    std::cout << std::endl;

    auto v2 = v.begin();
    for (; v2 != v.end(); ++v2) {
        std::cout << *v2 << " ";
    }
    std::cout << std::endl;

    auto minus_func = [](int a, int b) { return a - b; };

    std::vector<std::function<int(int, int)>> funcVec = {
        add_func,
        minus_func
    };

    d2x_assert_eq(funcVec[0](1, 2), 3);
    d2x_assert_eq(funcVec[1](1, 2), -1);

    return 0;
}
