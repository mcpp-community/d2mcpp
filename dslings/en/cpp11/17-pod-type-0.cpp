// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp11/17-pod-type-0.cpp
//
// Exercise: cpp11 | 17 - POD Type | Basic type properties
//
// Tips:
//  - Use std::is_pod / std::is_trivial / std::is_standard_layout to inspect type traits
//  - Compare how different struct definitions affect whether a type is POD
//
// Docs:
//   - https://en.cppreference.com/w/cpp/named_req/PODType
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/17-pod-type.md
//
// Discussion: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker pod-type

#include <d2x/cpp/common.hpp>

#include <type_traits>

struct A {
    int x;
    double y;
};

struct B {
    A a;
    int z;
};

struct C {
    virtual void foo();
    int x;
};

struct D {
    int x;
private:
    int y;
};

int main() {
    // TODO: use static_assert to check which of A/B/C/D are POD types.
    // 1. Check if A/B are POD
    D2X_YOUR_ANSWER;

    // 2. Check that C/D are not POD
    D2X_YOUR_ANSWER;

    // 3. Check if A is trivial / standard_layout type, write the result to ok
    bool ok = D2X_YOUR_ANSWER;
    d2x_assert(ok);

    // 4. Check if B is trivial / standard_layout type, write the result to ok
    ok = D2X_YOUR_ANSWER;
    d2x_assert(ok);

    // 5. Check if C is trivial type, write the result to ok
    ok = D2X_YOUR_ANSWER;
    d2x_assert(ok);

    // 6. Check if D is not standard_layout type, write the result to ok
    ok = D2X_YOUR_ANSWER;
    d2x_assert(!ok);

    D2X_WAIT
}


