// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp14/tests/00-generic-lambdas/0.cpp
//
// Exercise: cpp14 | 00 - generic lambdas | basic generic lambda
//
// Tips:
// - Use auto in lambda parameters; the compiler generates an implicit
//   template for operator()
// - The same generic lambda can accept arguments of different types
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/lambda
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/00-generic-lambdas.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker generic-lambdas
//

import std;
import d2x;

int main() {

    // 0. Simple generic lambda — identity
    auto identity = [](D2X_YOUR_ANSWER x) {
        return x;
    };

    d2x::check_eq(identity(42), 42, "identity(42) == 42");
    d2x::check(identity(std::string("hello")) == "hello", "identity(std::string(\"hello\")) == \"hello\"");
    d2x::check_eq(identity(3.14), D2X_YOUR_ANSWER, "identity(3.14) == D2X_YOUR_ANSWER");

    // 1. Generic lambda as comparator
    auto greater = [](auto a, auto b) {
        return D2X_YOUR_ANSWER;
    };

    d2x::check(greater(5, 3), "greater(5, 3)");
    d2x::check(greater(2.5, 1.2), "greater(2.5, 1.2)");
    d2x::check(greater(std::string("z"), std::string("a")), "greater(std::string(\"z\"), std::string(\"a\"))");

    // 2. Verify deduced types
    auto get_type_size = [](auto x) {
        return sizeof(D2X_YOUR_ANSWER);
    };

    d2x::check_eq(get_type_size(42), sizeof(int), "get_type_size(42) == sizeof(int)");
    d2x::check_eq(get_type_size('c'), sizeof(char), "get_type_size('c') == sizeof(char)");

    d2x::wait();
    return 0;
}
