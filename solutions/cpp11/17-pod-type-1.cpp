// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/17-pod-type-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/17-pod-type-1.cpp
//

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

    // 使用 memcpy 在 POD 上做按字节拷贝
    std::memcpy(&p2, &p1, sizeof(Packet));

    d2x_assert_eq(p2.len, 42u);
    d2x_assert_eq(p2.type, static_cast<std::uint16_t>(1));
    d2x_assert_eq(p2.flags, static_cast<std::uint16_t>(0xFF));

    return 0;
}
