// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp11/17-pod-type-1.cpp
//
// Exercise: cpp11 | 17 - POD Type | Byte-wise copying of a POD struct
//
// Tips:
//  - POD structs can be safely copied with std::memcpy within the same process
//  - Note: cross-process or cross-platform copying still needs to consider ABI and endianness
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/classes
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/17-pod-type.md
//
// Discussion: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker pod-type

#include <d2x/cpp/common.hpp>

#include <cstdint>
#include <cstring>

struct Packet {
    std::uint32_t len;
    std::uint16_t type;
    std::uint16_t flags;
}; // POD

int main() {
    Packet p1{};
    p1.len = 42;
    p1.type = 1;
    p1.flags = 0xFF;

    Packet p2{};

    // TODO: use std::memcpy to copy the contents of p1 into p2.
    D2X_YOUR_ANSWER;

    d2x_assert_eq(p2.len, 42u);
    d2x_assert_eq(p2.type, static_cast<std::uint16_t>(1));
    d2x_assert_eq(p2.flags, static_cast<std::uint16_t>(0xFF));

    D2X_WAIT

    return 0;
}


