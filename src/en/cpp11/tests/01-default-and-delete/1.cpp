// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/01-default-and-delete/1.cpp
//
// Exercise: cpp11 | 01 - default and delete | Non-copyable objects
//
// Tips: Use `= default` and `= delete` to fix errors based on compiler hints
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/function#Function_definition
//   - https://en.cppreference.com/w/cpp/language/function#Deleted_functions
//
// Auto-Checker command:
//
//   d2x checker default-and-delete-1
//

import std;
import d2x;


// Implement std::unique_ptr property: not copyable but movable
struct UniquePtr {
    void *dataPtr;
    UniquePtr() = default;
};

int main() { // Do not directly modify the code in the main function

    // std::unique_ptr<int> a(new int(1));
    UniquePtr a;

    // Object cannot be copied/duplicated
    // std::unique_ptr<int> b = a; // error
    d2x::check(std::is_copy_constructible<UniquePtr>::value == false, "std::is_copy_constructible<UniquePtr>::value == false");
    // a = b; // error
    d2x::check(std::is_copy_assignable<UniquePtr>::value == false, "std::is_copy_assignable<UniquePtr>::value == false");

    // Object can be moved
    // std::unique_ptr<int> c = std::move(a); // ok
    d2x::check(std::is_move_constructible<UniquePtr>::value == true, "std::is_move_constructible<UniquePtr>::value == true");
    // a = std::move(c); // ok
    d2x::check(std::is_move_assignable<UniquePtr>::value == true, "std::is_move_assignable<UniquePtr>::value == true");

    d2x::wait();
    return 0;
}