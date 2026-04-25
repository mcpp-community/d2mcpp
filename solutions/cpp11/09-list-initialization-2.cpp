// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/09-list-initialization-2.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/09-list-initialization-2.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <vector>
#include <initializer_list>

class MyVector {
    int mSize;

public:
    MyVector(std::initializer_list<int> il) : mSize(static_cast<int>(il.size())) {}

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

    return 0;
}
