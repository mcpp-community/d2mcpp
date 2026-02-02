// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/05-move-semantics-0.cpp
//
// Exercise/练习: cpp11 | 05 - move semantics | 移动构造与触发时机
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

struct Buffer {
    int *data;
    Buffer() : data { new int[2] {0, 1} } {
        std::cout << "Buffer():" << data << std::endl;
    }
    Buffer(const Buffer &other)  {
        std::cout << "Buffer(const Buffer&):" << data << std::endl;
        data = new int[2];
        data[0] = other.data[0];
        data[1] = other.data[1];
    }
    Buffer(Buffer&& other) : data { other.data } { 
        std::cout << "Buffer(Buffer&&):" << data << std::endl;
        other.data = nullptr; // 让原对象的指针失效
    }
    ~Buffer() {
        if (data) {
            std::cout << "~Buffer():" << data << std::endl;
            delete[] data;
        }
    }
    const int * data_ptr() const { return data; }
};

Buffer process(Buffer buff) {
    std::cout << "process(): " << buff.data << std::endl;
    return buff;
}

int main() {
    {
        Buffer buff1 = process(Buffer());
        auto buff1DataPtr = buff1.data_ptr();

        std::cout << " --- " << std::endl;

        Buffer buff2(std::move(buff1));
        auto buff2DataPtr = buff2.data_ptr();

        d2x_assert(buff1DataPtr == buff2DataPtr);

        Buffer buff3 = buff2;
        auto buff3DataPtr = buff3.data_ptr();

        d2x_assert(buff2DataPtr == buff3DataPtr);

        Buffer buff4 = process(buff3);
        auto buff4DataPtr = buff4.data_ptr();

        d2x_assert(buff3DataPtr == buff4DataPtr);
    }

    D2X_WAIT

    return 0;
}