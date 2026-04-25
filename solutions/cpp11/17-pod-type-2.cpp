// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/17-pod-type-2.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/17-pod-type-2.cpp
//

#include <d2x/cpp/common.hpp>

#include <cstdint>
#include <string>


// C 风格的消息头部，仅包含 POD 成员，用于跨语言/跨模块传递
struct MessageHeader {
    std::uint32_t len;   // payload 长度（字节数）
    std::uint16_t type;  // 消息类型
    std::uint16_t flags; // 预留标志位
}; // POD

// C++ 侧更友好的消息类型，可以使用 std::string 等非 POD 成员
struct Message {
    std::uint16_t type;
    std::string   payload;
};

// 模拟的 C 接口：接收二进制头部数据
static MessageHeader g_last_header{};

extern "C" void c_send_header(const void* data, std::size_t size) {
    d2x_assert_eq(size, sizeof(MessageHeader));
    const auto* header = static_cast<const MessageHeader*>(data);
    g_last_header = *header;
}

void send_message_to_c(const Message& msg) {
    MessageHeader header;
    header.len = static_cast<std::uint32_t>(msg.payload.size());
    header.type = msg.type;
    header.flags = 0;
    c_send_header(&header, sizeof(header));
}

int main() {
    Message msg{};
    msg.type = 42;
    msg.payload = "hello pod";

    send_message_to_c(msg);

    // 验证 C 接口收到的头部内容是否正确
    d2x_assert_eq(g_last_header.type, msg.type);
    d2x_assert_eq(g_last_header.len, static_cast<std::uint32_t>(msg.payload.size()));
    d2x_assert_eq(g_last_header.flags, static_cast<std::uint16_t>(0));

    return 0;
}
