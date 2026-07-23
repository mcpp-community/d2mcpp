// NDJSON 事件输出。
//
// 协议要求每一行都是一个完整的 JSON 对象，解析不了的行由 d2x 忽略——
// 这正好吞掉 `mcpp run` 的前导空行和可能混入的编译输出，所以这里不需要
// 任何哨兵前缀。唯一的硬要求是：一个事件绝不能跨行。
module;

// stdout / fputs / fflush 是宏与 C 运行时符号，import std 不提供，
// 必须走全局模块片段。
#include <cstdio>

export module d2x.provider.emit;

import std;

namespace d2x::emit {

// JSON 字符串转义。练习输出里有 ANSI 转义序列、换行、引号，必须全部处理，
// 否则一个彩色编译错误就能把整行 JSON 打碎。
export std::string escape(std::string_view s) {
    std::string out;
    out.reserve(s.size() + s.size() / 8);
    for (unsigned char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            case '\b': out += "\\b";  break;
            case '\f': out += "\\f";  break;
            default:
                // ANSI 的 ESC(0x1b) 落在这里，按  编码
                if (c < 0x20) out += std::format("\\u{:04x}", static_cast<int>(c));
                else          out += static_cast<char>(c);
        }
    }
    return out;
}

export std::string str(std::string_view v) {
    return std::format("\"{}\"", escape(v));
}

export std::string array(const std::vector<std::string>& items) {
    std::string out = "[";
    for (std::size_t i = 0; i < items.size(); ++i) {
        if (i) out += ",";
        out += items[i];
    }
    out += "]";
    return out;
}

// 一行一个事件，立刻 flush——d2x 是逐行读的，缓冲会让「实时看到编译输出」失效。
export void line(std::string_view json_object) {
    std::fputs(std::string(json_object).c_str(), stdout);
    std::fputc('\n', stdout);
    std::fflush(stdout);
}

export void describe(std::string_view name, int protocol) {
    line(std::format(R"({{"event":"describe","protocol":{},"name":{}}})",
                     protocol, str(name)));
}

export void exercise(std::string_view id, int order, std::string_view title,
                     std::string_view chapter, const std::vector<std::string>& files) {
    std::vector<std::string> quoted;
    quoted.reserve(files.size());
    for (const auto& f : files) quoted.push_back(str(f));

    line(std::format(R"({{"event":"exercise","id":{},"order":{},"title":{},"chapter":{},"files":{}}})",
                     str(id), order, str(title), str(chapter), array(quoted)));
}

export void stage(std::string_view name) {
    line(std::format(R"({{"event":"stage","name":{}}})", str(name)));
}

export void output(std::string_view chunk) {
    if (chunk.empty()) return;
    line(std::format(R"({{"event":"output","chunk":{}}})", str(chunk)));
}

export void verdict(std::string_view outcome, std::string_view stage_name, int exit_code,
                    const std::vector<std::string>& diagnostics = {}) {
    line(std::format(R"({{"event":"verdict","outcome":{},"stage":{},"exit_code":{},"diagnostics":{}}})",
                     str(outcome), str(stage_name), exit_code, array(diagnostics)));
}

// 一条失败的断言 → 一个 Diagnostic。d2x 的前端据此做行内高亮和跳转，
// 学习者不用在几十行输出里找是哪一条没过。
export std::string diagnostic(std::string_view file, int line_no,
                              std::string_view expr,
                              std::string_view expected, std::string_view actual) {
    auto message = expected.empty() && actual.empty()
        ? std::format("断言未通过: {}", expr)
        : std::format("断言未通过: {} —— 期望 {}，实际 {}", expr, expected, actual);

    return std::format(
        R"({{"file":{},"line":{},"col":0,"severity":"error","message":{}}})",
        str(file), line_no, str(message));
}

export void error(std::string_view message) {
    line(std::format(R"({{"event":"error","message":{}}})", str(message)));
}

} // namespace d2x::emit
