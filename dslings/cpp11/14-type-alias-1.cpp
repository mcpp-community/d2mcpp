// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/14-type-alias-1.cpp
//
// Exercise/练习: cpp11 | 14 - type alias | 复杂类型和函数指针别名
//
// Tips/提示: 使用using关键字为复杂类型和函数指针定义别名
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/type_alias
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/14-type-alias.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker type-alias
//

#include <d2x/cpp/common.hpp>

#include <vector>
#include <string>

static int func_called = 0;

// 函数声明
void example_func(int a, int b) {
    func_called = a + b;
}

int main() {

    // 1. 函数指针别名
    D2X_YOUR_ANSWER FuncPtr = void(*)(int, int);

    FuncPtr func = example_func;

    func(1, 2);
    d2x_assert_eq(func_called, 3);

    // 2. 容器类型别名
    // 使用using定义vector<string>的别名
    D2X_YOUR_ANSWER = std::vector<std::string>;

    // 使用容器类型别名
    StringVector strings = {"hello", "world"};

    // 3. 嵌套类型别名
    struct Container {
        ValueType = int
    };

    Container::ValueType value = 100;

    // 4. 验证类型别名
    d2x_assert(strings[0] == "hello");
    d2x_assert(strings[1] == "world");
    d2x_assert_eq(value, 100);

    D2X_WAIT

    return 0;
}
