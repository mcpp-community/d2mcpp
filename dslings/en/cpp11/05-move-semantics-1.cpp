// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/05-move-semantics-1.cpp
//
// Exercise: cpp11 | 05 - move semantics | Move Assignment and Trigger Timing
//
// Tips: Observe compiler output, without changing the buff passing logic, ensure only one resource allocation and deallocation
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/move_constructor
//
// Auto-Checker command:
//
//   d2x checker move-semantics
//

#include <d2x/common.hpp>

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
        other.data = nullptr; // Invalidate the pointer of the original object
    }
    Buffer & operator=(const Buffer &other) {
        std::cout << "Buffer& operator=(const Buffer&):" << data << std::endl;
        if (this != &other) {
            delete[] data; // Release old resources
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
            delete[] data; // Release old resources
            data = other.data; // Transfer resources
            other.data = nullptr; // Invalidate the pointer of the original object
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

int main() { // No compiler optimization
 
    {
        Buffer buff1;

        buff1 = Buffer(); // Case 1: Temporary object assignment

        d2x_assert_eq(move_assignment_counter, 1);

        Buffer buff2;

        buff2 = process(buff1); // Case 2: Intermediate object assignment

        d2x_assert_eq(move_assignment_counter, 2);

        buff2 = buff1; // Case 3: Explicit move assignment

        d2x_assert_eq(move_assignment_counter, 3);

    }

    D2X_WAIT

    return 0;
}