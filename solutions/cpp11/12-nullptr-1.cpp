// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/12-nullptr-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/12-nullptr-1.cpp
//

#include <d2x/cpp/common.hpp>

bool process_int_called = false;
bool process_ptr_called = false;
bool display_int_called = false;
bool display_ptr_called = false;

void process(int* ptr) {
    process_ptr_called = true;
    (void)ptr;
}

void process(int value) {
    process_int_called = true;
    (void)value;
}

void display(int* ptr) {
    display_ptr_called = true;
    (void)ptr;
}

void display(int value) {
    display_int_called = true;
    (void)value;
}

int main() {

    display(0);          // -> display(int)
    process(nullptr);    // -> process(int*)
    display(nullptr);    // 用 nullptr 准确选中 display(int*)
    process(0);          // -> process(int)

    d2x_assert(process_int_called);
    d2x_assert(display_int_called);
    d2x_assert(display_ptr_called);
    d2x_assert(display_ptr_called);

    return 0;
}
