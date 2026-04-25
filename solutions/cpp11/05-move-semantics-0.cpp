// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/05-move-semantics-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/05-move-semantics-0.cpp
//
// 教学要点: 让 Buffer 在拷贝时也"转移"资源, 只做一次资源分配和释放,
// 这样每个传递点最终都指向同一块缓冲区, 三个 d2x_assert 都通过。

#include <d2x/cpp/common.hpp>

#include <iostream>

struct Buffer {
    int *data;
    Buffer() : data { new int[2] {0, 1} } {
        std::cout << "Buffer():" << data << std::endl;
    }
    // 让 "拷贝" 也变成转移资源, 保证只做一次分配/释放
    Buffer(const Buffer &other) {
        std::cout << "Buffer(const Buffer&):" << other.data << std::endl;
        data = other.data;
        const_cast<Buffer&>(other).data = nullptr;
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

    return 0;
}
