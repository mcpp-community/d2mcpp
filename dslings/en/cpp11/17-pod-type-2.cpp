// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp11/17-pod-type-2.cpp
//
// Exercise: cpp11 | 17 - POD Type | Adapting a C++ message to a C POD header
//
// Tips:
//  - C interfaces typically accept only raw pointers + sizes (void* + size_t) and expect POD/standard-layout data
//  - C++ code can use richer types (e.g. std::string) and adapt them to a separate POD header when calling C
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

#include <cstdint>
#include <string>

// C-style message header, POD-only, used for cross-language / cross-module transfer.
struct MessageHeader {
    std::uint32_t len;   // payload length in bytes
    std::uint16_t type;  // message type
    std::uint16_t flags; // reserved flags
}; // POD

// C++-side user-friendly message type, may use non-POD members like std::string.
struct Message {
    std::uint16_t type;
    std::string   payload;
};

// Simulated C interface: receives binary header data.
static MessageHeader g_last_header{};

extern "C" void c_send_header(const void* data, std::size_t size) {
    // In a real-world scenario, this might be a C library or system call.
    d2x_assert_eq(size, sizeof(MessageHeader));
    const auto* header = static_cast<const MessageHeader*>(data);
    g_last_header = *header;
}

// TODO:
//  Implement this function:
//  1. Build a MessageHeader from the Message (len = payload.size(), type = msg.type, flags = 0).
//  2. Call c_send_header to pass the POD header as raw bytes to the C interface.
void send_message_to_c(const Message& msg) {
    D2X_YOUR_ANSWER
}

int main() {
    Message msg{};
    msg.type = 42;
    msg.payload = "hello pod";

    send_message_to_c(msg);

    // Verify that the C interface received the correct header contents.
    d2x_assert_eq(g_last_header.type, msg.type);
    d2x_assert_eq(g_last_header.len, static_cast<std::uint32_t>(msg.payload.size()));
    d2x_assert_eq(g_last_header.flags, static_cast<std::uint16_t>(0));

    D2X_WAIT

    return 0;
}


