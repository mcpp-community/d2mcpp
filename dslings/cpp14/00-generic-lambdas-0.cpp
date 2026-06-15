// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp14/00-generic-lambdas-0.cpp
//
// Exercise/练习: cpp14 | 00 - generic lambdas | 泛型 lambda
//
// Tips/提示:
// - lambda 参数使用 auto, 编译器为 operator() 生成隐式模板
// - 同一个泛型 lambda 可以接受不同类型的参数
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/lambda
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/00-generic-lambdas.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker generic-lambdas
//

#include <d2x/cpp/common.hpp>
#include <string>

int main() {

    // 0. 简单泛型 lambda — identity
    auto identity = [](D2X_YOUR_ANSWER x) {
        return x;
    };

    d2x_assert_eq(identity(42), 42);
    d2x_assert(identity(std::string("hello")) == "hello");
    d2x_assert_eq(identity(3.14), D2X_YOUR_ANSWER);

    // 1. 泛型 lambda 做比较
    auto greater = [](auto a, auto b) {
        return D2X_YOUR_ANSWER;
    };

    d2x_assert(greater(5, 3));
    d2x_assert(greater(2.5, 1.2));
    d2x_assert(greater(std::string("z"), std::string("a")));

    // 2. 推导类型确认
    auto get_type_size = [](auto x) {
        return sizeof(D2X_YOUR_ANSWER);
    };

    d2x_assert_eq(get_type_size(42), sizeof(int));
    d2x_assert_eq(get_type_size('c'), sizeof(char));

    D2X_WAIT

    return 0;
}
