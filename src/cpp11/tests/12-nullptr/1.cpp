// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/cpp11/tests/12-nullptr/1.cpp
//
// Exercise/练习: cpp11 | 12 - nullptr | 指针字面量 - 函数重载
//
// Tips/提示: 使用 nullptr 解决函数重载中的歧义问题
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/nullptr
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/12-nullptr.md
//
// 练习交流讨论: http://forum.d2learn.org
//
// Auto-Checker/自动检测命令:
//
//   d2x checker nullptr
//

import std;
import d2x;

bool process_int_called = false;
bool process_ptr_called = false;
bool display_int_called = false;
bool display_ptr_called = false;

void process(int* ptr) {
    process_ptr_called = true;
}

void process(int value) {
    process_int_called = true;
}

void display(int* ptr) {
    display_ptr_called = true;
}

void display(int value) {
    display_int_called = true;
}

int main() {

    display(0);
    process(nullptr);
    display(NULL);
    process(NULL);

    d2x::check(process_int_called, "process_int_called");
    d2x::check(display_int_called, "display_int_called");
    d2x::check(display_ptr_called, "display_ptr_called");
    d2x::check(display_ptr_called, "display_ptr_called");

    d2x::wait();
    return 0;
}
