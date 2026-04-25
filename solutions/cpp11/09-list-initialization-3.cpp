// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/09-list-initialization-3.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/09-list-initialization-3.cpp
//
// 教学要点:
//   1. 用户提供构造函数的类型不再是聚合, Point 需要显式定义 (int, int) 构造函数
//      才能用 `{1, 2}` 这种形式构造。
//   2. 列表初始化优先选 std::initializer_list 重载, 这里去掉 initializer_list 重载,
//      让 `MyVector vec4 { 1, 10 }` 命中 (int, int) 构造函数。

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <vector>

class MyVector {
    int mSize;
    int *data;
public:
    MyVector(int val) {
        mSize = 1;
        data = new int[mSize];
        data[0] = val;
    }

    MyVector(int v1, int sz) {
        mSize = sz;
        data = new int[mSize];
        for (int i = 0; i < sz; ++i) {
            data[i] = v1;
        }
    }

    // 注意: 移除 initializer_list 构造函数, 否则 `MyVector{1, 10}` 会优先匹配
    // initializer_list, 导致 size() 等于 2 而非 10.

    MyVector(const MyVector& other) = delete;
    MyVector(MyVector&& other) = delete;

    ~MyVector() {
        delete[] data;
    }

    int size() const {
        return mSize;
    }
};

struct Point {
    int x, y;

    Point() : x {0}, y{0} { }
    Point(int x_, int y_) : x{x_}, y{y_} { } // 添加 (int, int) 构造函数
};

int main() {

    Point p1 = {1, 2};
    Point p2 {3, 4};
    (void)p1; (void)p2;

    MyVector vec1(1);
    d2x_assert_eq(vec1.size(), 1);
    MyVector vec2 { 1 };
    d2x_assert_eq(vec2.size(), 1);

    MyVector vec3(1, 10);
    d2x_assert_eq(vec3.size(), 10);
    MyVector vec4 { 1, 10 };
    d2x_assert_eq(vec4.size(), 10);

    return 0;
}
