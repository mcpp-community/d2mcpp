// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/cpp11/tests/09-list-initialization/2.cpp
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

import std;
import d2x;


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
    d2x::check_eq(vec1.size(), 3, "vec1.size() == 3");
    std::vector<int> vec2 { 1, 2, 3, 4, 5 };
    d2x::check_eq(vec2.size(), 5, "vec2.size() == 5");

    MyVector myVec1 = { 1, 2, 3 };
    d2x::check_eq(myVec1.size(), 3, "myVec1.size() == 3");
    MyVector myVec2 { 1, 2, 3, 4, 5 };
    d2x::check_eq(myVec2.size(), 5, "myVec2.size() == 5");

    d2x::wait();
    return 0;
}