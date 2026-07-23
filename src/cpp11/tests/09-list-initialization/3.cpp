// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/cpp11/tests/09-list-initialization/3.cpp
//
// Exercise/练习: cpp11 | 09 - list initialization | 注意事项
//
// Tips/提示: 修复编译器报错, 了解列表初始化的注意事项
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

    MyVector(std::initializer_list<int> list) {
        mSize = list.size();
        data = new int[mSize];
        int i = 0;
        for (const auto& val : list) {
            data[i++] = val;
        }
    }

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
};

int main() {

    Point p1 = {1, 2};
    Point p2 {3, 4};

    MyVector vec1(1);
    d2x::check_eq(vec1.size(), 1, "vec1.size() == 1");
    MyVector vec2 { 1 };
    d2x::check_eq(vec2.size(), 1, "vec2.size() == 1");

    MyVector vec3(1, 10);
    d2x::check_eq(vec3.size(), 10, "vec3.size() == 10");
    MyVector vec4 { 1, 10 };
    d2x::check_eq(vec4.size(), 10, "vec4.size() == 10");

    d2x::wait();
    return 0;
}