// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/09-list-initialization-2.cpp
//
// Exercise/练习: cpp11 | 09 - list initialization | 容器列表初始化
//
// Tips/提示: 实现一个支持列表初始化的构造函数, 计算容器大小, 了解initializer_list并通过运行时检查
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/list_initialization.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/09-list-initialization.md
//   - https://en.cppreference.com/w/cpp/utility/initializer_list.html
//
// Auto-Checker/自动检测命令:
//
//   d2x checker list-initialization
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <vector>
#include <initializer_list>

class MyVector {
    int mSize;

public:
    D2X_YOUR_ANSWER

    int size() const {
        return mSize;
    }
};

int main() {

    std::vector<int> vec1 = { 1, 2, 3 };
    d2x_assert_eq(vec1.size(), 3);
    std::vector<int> vec2 { 1, 2, 3, 4, 5 };
    d2x_assert_eq(vec2.size(), 5);

    MyVector myVec1 = { 1, 2, 3 };
    d2x_assert_eq(myVec1.size(), 3);
    MyVector myVec2 { 1, 2, 3, 4, 5 };
    d2x_assert_eq(myVec2.size(), 5);

    D2X_WAIT

    return 0;
}