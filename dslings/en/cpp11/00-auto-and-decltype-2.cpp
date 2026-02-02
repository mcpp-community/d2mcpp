// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-2.cpp
//
// Exercise: cpp11 | 00 - auto and decltype | Complex Type Deduction
//
// Tips: Use auto and decltype to fix errors in the code
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/auto
//   - https://en.cppreference.com/w/cpp/language/decltype
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/00-auto-and-decltype.md
//
// Forum: https://forum.d2learn.org/category/20
//
// Auto-Checker command:
//
//   d2x checker auto-and-decltype-2
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <vector>
#include <functional>

int add_func(int a, int b) {
    return a + b;
}

int main() {

    // 2. Complex types

    std::vector<int> v = {1, 2, 3};

    std::vector<int>::iterator v1 = v.begin();
    for (; v1 != v.end(); ++v1) {
        std::cout << *v1 << " ";
    }
    std::cout << std::endl;

    D2X_YOUR_ANSWER v2 = v.begin();
    for (; v2 != v.end(); ++v2) {
        std::cout << *v2 << " ";
    }
    std::cout << std::endl;

    auto minus_func = [](int a, int b) { return a - b; };

    std::vector<std::function<D2X_YOUR_ANSWER>> funcVec = {
        add_func,
        minus_func
    };

    d2x_assert_eq(funcVec[0](1, 2), 3);
    d2x_assert_eq(funcVec[1](1, 2), -1);

    D2X_WAIT

    return 0;
}
