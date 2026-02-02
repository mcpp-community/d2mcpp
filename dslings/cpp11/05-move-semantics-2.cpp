// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/05-move-semantics-2.cpp
//
// Exercise/练习: cpp11 | 05 - move semantics | 移动的是资源而不是对象
//
// Tips/提示: 观察编译器输出, 使用移动语义并观察对比对象和资源的移动
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/move_constructor
//
// Auto-Checker/自动检测命令:
//
//   d2x checker move-semantics-2
//

#include <d2x/common.hpp>

#include <iostream>

struct Buffer {
    int *data;
    Buffer() : data { new int[2] {0, 1} } {
        std::cout << "Buffer():" << data << std::endl;
    }
    Buffer(const Buffer &other) {
        std::cout << "Buffer(const Buffer&):" << data << std::endl;
        data = new int[2];
        data[0] = other.data[0];
        data[1] = other.data[1];
    }
    Buffer(Buffer&& other) : data { other.data } {
        std::cout << "Buffer(Buffer&&):" << data << std::endl;
        other.data = nullptr; // 让原对象的指针失效
    }
    Buffer & operator=(const Buffer &other) {
        std::cout << "Buffer& operator=(const Buffer&):" << data << std::endl;
        if (this != &other) {
            delete[] data; // 释放旧资源
            data = new int[2];
            data[0] = other.data[0];
            data[1] = other.data[1];
        }
        return *this;
    }
    Buffer & operator=(Buffer&& other) {
        std::cout << "Buffer& operator=(Buffer&&):" << data << std::endl;
        if (this != &other) {
            delete[] data; // 释放旧资源 - 步骤1
            data = other.data; // 转移资源 - 步骤2
            other.data = nullptr; // 让原对象的指针失效 - 步骤3
        }
        return *this;
    }
    ~Buffer() {
        if (data) {
            std::cout << "~Buffer():" << data << std::endl;
            delete[] data;
        }
    }
    const int * data_ptr() const {
        return data;
    }
};

int main() { // 移动语义 - 移动的是资源而不是对象演示
 
    {
        Buffer b1; // 调用默认构造函数

        auto old_b1_data_ptr = b1.data_ptr();

        Buffer b2 = b1; // std::move(b1);

        d2x_assert(&b1 != &b2); // b1 和 b2 是不同的对象
        d2x_assert(old_b1_data_ptr == b2.data_ptr());
        d2x_assert(b1.data_ptr() == nullptr); // b1 的资源被移动了

    }

    D2X_WAIT

    return 0;
}