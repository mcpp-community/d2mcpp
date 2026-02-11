// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/17-pod-type-2.cpp
//
// Exercise/练习: cpp11 | 17 - POD Type | 适配 C 接口的 POD 头部
//
// Tips/提示:
//  - C 接口通常只接受裸指针 + 大小（void* + size_t），要求参数是 POD/标准布局
//  - C++ 代码可以使用更丰富的类型（如 std::string），通过单独的 POD 头部与 C 接口对接
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/named_req/PODType
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/17-pod-type.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker pod-type

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
    // 在真实场景中，这里可能是 C 库/系统调用，这里简化为记录收到的头部
    d2x_assert_eq(size, sizeof(MessageHeader));
    const auto* header = static_cast<const MessageHeader*>(data);
    g_last_header = *header;
}

// TODO:
//  实现此函数：
//  1. 根据 Message 构造一个 MessageHeader（len = payload.size(), type = msg.type, flags 可设为 0）。
//  2. 调用 c_send_header，将 POD 头部按二进制形式传给 C 接口。
void send_message_to_c(const Message& msg) {
    D2X_YOUR_ANSWER
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

    D2X_WAIT

    return 0;
}

