// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/04-rvalue-references/0.cpp
//
// Exercise: cpp11 | 04 - rvalue references
//
// Tips: Use the correct way to extend the lifetime of temporary objects so they can be modified again
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/reference
//
// Auto-Checker command:
//
//   d2x checker rvalue-references
//

import std;
import d2x;


struct Object;
static Object * object_address = nullptr;
static int move_ctor_calls = 0;   // 移动构造被调用的次数, 供断言检查

struct Object {
    int data = 0;
    Object() {
        std::cout << "Object():" << this << std::endl;
        object_address = this;
    }
    Object(const Object&) { std::cout << "Object(const Object&):" << this << std::endl; }
    Object(Object&&) { ++move_ctor_calls; std::cout << "Object(Object&&):" << this << std::endl; }
    ~Object() { std::cout << "~Object():" << this << std::endl; }
};

int main() { // Disable compiler optimization
    {
        std::cout << "----> Temporary object - rvalue 1" << std::endl;
        Object();
        std::cout << "----> Temporary object - rvalue 2 (named object + std::move)" << std::endl;
        // NOTE: `Object obj = Object();` will NOT show a move constructor.
        // Since C++17 a prvalue initialises the target directly (guaranteed
        // copy elision), and -fno-elide-constructors cannot bring that move
        // back. Moving from a named object is the standard-independent way.
        Object named;
        Object obj = std::move(named);

        std::cout << "--------Code modifiable area - Start--------" << std::endl;


        const Object &objRef = Object(); // Extend temporary object lifetime


        std::cout << "--------Code modifiable area - End--------" << std::endl;

        objRef.data = 1; // Modify the value of the extended lifetime temporary object (do not directly modify this line)
        std::cout << "objRef.data = " << objRef.data << " - " << &objRef << std::endl;
        d2x::check((&objRef == object_address), "(&objRef == object_address)");
        // 钉住移动构造确实发生过。教学漂移之所以能静默发生, 正是因为
        // 从前没有任何断言检查它 —— 输出少了一行, 没人发现。
        d2x::check((move_ctor_calls >= 1), "(move_ctor_calls >= 1)");
    }

    d2x::wait();
    return 0;
}