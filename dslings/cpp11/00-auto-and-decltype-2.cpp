// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/00-auto-and-decltype-2.cpp
//
// Exercise/练习: cpp11 | 00 - auto and decltype | 复杂类型推导
//
// Tips/提示: 使用 auto 和 decltype 修复代码中的错误
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/auto
//   - https://en.cppreference.com/w/cpp/language/decltype
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/00-auto-and-decltype.md
//
// 练习交流讨论: http://forum.d2learn.org/post/357
//
// Auto-Checker/自动检测命令:
//
//   d2x checker auto-and-decltype-2
//

#include <d2x/common.hpp>

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