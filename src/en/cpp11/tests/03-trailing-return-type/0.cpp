// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/03-trailing-return-type/0.cpp
//
// Exercise: cpp11 | 03 - trailing return type
//
// Tips: Replace D2X_YOUR_ANSWER with the correct type to pass all checks
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/function#Function_declaration
//
// Auto-Checker command:
//
//   d2x checker trailing-return-type
//

import std;
import d2x;


int add0(double a, int b) {
    return a + b;
}

auto add1(double a, int b) -> int {
    return a + b;
}

template<typename T1, typename T2>
auto add2(const T1 &a, const T2 &b) -> D2X_YOUR_ANSWER {
    return a + b;
}

auto add3 = [](double a, double b) -> D2X_YOUR_ANSWER {
    return a + b;
};

int main() {

    d2x::check_eq(add0(1.1, 2), 3, "add0(1.1, 2) == 3");
    d2x::check_eq(add1(1.1, 2), 3, "add1(1.1, 2) == 3");
    d2x::check_eq(add2(1.1, 2), 3.1, "add2(1.1, 2) == 3.1");
    d2x::check_eq(add2(1, 2.1), 3.1, "add2(1, 2.1) == 3.1");
    d2x::check_eq(add3(1.1, 2.1), 3, "add3(1.1, 2.1) == 3");

    d2x::wait();
    return 0;
}