// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/05-move-semantics-2.cpp
//
// Exercise: cpp11 | 05 - move semantics | Moving resources not objects
//
// Tips: Observe compiler output, use move semantics and observe the movement of objects vs resources
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/move_constructor
//
// Auto-Checker command:
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
        std::cout << "Buffer& operator=(Buffer&&):" << data << std::endl;
        if (this != &other) {
            delete[] data; // Release old resources - Step 1
            data = other.data; // Transfer resources - Step 2
            other.data = nullptr; // Invalidate the pointer of the original object - Step 3
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

int main() { // Move semantics - Demonstrating moving resources not objects
 
    {
        Buffer b1; // Call default constructor

        auto old_b1_data_ptr = b1.data_ptr();

        Buffer b2 = b1; // std::move(b1);

        d2x_assert(&b1 != &b2); // b1 and b2 are different objects
        d2x_assert(old_b1_data_ptr == b2.data_ptr());
        d2x_assert(b1.data_ptr() == nullptr); // b1's resources have been moved

    }

    D2X_WAIT

    return 0;
}