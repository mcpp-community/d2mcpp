// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/05-move-semantics-0.cpp
//
// Exercise/练习: cpp11 | 05 - move semantics | 移动赋值与触发时机
//
// Tips/提示: 观察编译器输出, 在不改变buff传递的逻辑, 使得只做一次资源的分配和释放
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/move_constructor
//
// Auto-Checker/自动检测命令:
//
//   d2x checker move-semantics
//

#include <d2x/cpp/common.hpp>

#include <iostream>


static int move_assignment_counter = 0;

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
        move_assignment_counter++;
        std::cout << "Buffer& operator=(Buffer&&):" << data << std::endl;
        if (this != &other) {
            delete[] data; // 释放旧资源
            data = other.data; // 转移资源
            other.data = nullptr; // 让原对象的指针失效
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
        std::cout << "data[0] = " << data[0] << ", data[1] = " << data[1] << std::endl;
        return data;
    }
};

Buffer process(Buffer buff) {
    std::cout << "process(): " << buff.data << std::endl;
    return buff;
}

int main() { // 无编译器优化
 
    {
        Buffer buff1;

        buff1 = Buffer(); // 情况1: 临时对象赋值

        d2x_assert_eq(move_assignment_counter, 1);

        Buffer buff2;

        buff2 = process(buff1); // 情况2: 中间对象赋值

        d2x_assert_eq(move_assignment_counter, 2);

        buff2 = buff1; // 情况3: 显示移动赋值

        d2x_assert_eq(move_assignment_counter, 3);

    }

    D2X_WAIT

    return 0;
}