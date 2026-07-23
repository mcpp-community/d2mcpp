// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/16-generalized-unions/0.cpp
//
// Exercise: cpp11 | 16 - generalized unions | generalized (non-trivial) unions
//
// Tips:
// - Allowing the initialization of a member variable
//
// Docs:
//   - https://cppreference.com/w/cpp/language/union.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/16-generalized-unions.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker generalized-unions
//

import std;
import d2x;


union M
{
    int a1 = D2X_YOUR_ANSWER;
    int a2 = 21;    // Remove redundant initializations
    D2X_YOUR_ANSWER a3;
    char c;
};


int main() {

    M u1;
    //Initialize union members
    d2x::check(u1.a1 == 42, "u1.a1 == 42");
    u1.a2 = 21;

    double val = 3.14;
    u1.a3 = val;

    u1.c = 'x';

    d2x::wait();
    return 0;
}
