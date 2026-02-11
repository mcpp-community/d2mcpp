// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/17-pod-type-1.cpp
//
// Exercise/练习: cpp11 | 17 - POD Type | 按字节拷贝 POD 结构体
//
// Tips/提示:
//  - POD 结构体可以安全地使用 std::memcpy 在同一进程内按字节拷贝
//  - 注意：跨进程或跨平台时仍需要考虑 ABI、字节序等问题
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/classes
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/17-pod-type.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
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

    // TODO: 使用内存拷贝将 p1 的内容复制到 p2
    D2X_YOUR_ANSWER;

    d2x_assert_eq(p2.len, 42u);
    d2x_assert_eq(p2.type, static_cast<std::uint16_t>(1));
    d2x_assert_eq(p2.flags, static_cast<std::uint16_t>(0xFF));

    D2X_WAIT

    return 0;
}

