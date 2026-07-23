// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/12-nullptr/1.cpp
//
// Exercise: cpp11 | 12 - nullptr | Pointer Literal - Function Overloading
//
// Tips: Use nullptr to resolve ambiguity in function overloading
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/nullptr
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/cpp11/12-nullptr.md
//
// Exercise discussion: http://forum.d2learn.org/post/359
//
// Auto-Checker command:
//
//   d2x checker nullptr
//

#include <cstddef>   // NULL 宏是这一课的教具,import std 不带宏
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