#ifndef REPORT_HPP_D2X
#define REPORT_HPP_D2X

// 侧信道上报：把判定信号从 stdout 移到带外。
//
// 为什么需要：断言失败并不改变退出码，所以判定只能靠扫 stdout 找 ❌ ——
// 这会误判。一个断言全过的正确解答，只要在说明文字里打了个 ❌ 就被判失败
// （已复现）。反过来，输出被截断或 Windows 控制台代码页不对，判定同样失效。
//
// 于是：stdout 回归「给人看」，判定走这个文件。
//
// 逐条追加而非退出时统一写 —— 练习段错误时，崩溃之前的断言结果照样保留，
// 学员仍能看到前几条过了。
//
// D2X_RESULT_FILE 未设置时这里什么都不做：学员直接跑二进制零摩擦。

#include <cstdio>
#include <cstdlib>
#include <string>

namespace d2x::report {

inline std::string escape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (unsigned char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:
                if (c < 0x20) {
                    char buf[8];
                    std::snprintf(buf, sizeof buf, "\\u%04x", static_cast<int>(c));
                    out += buf;
                } else {
                    out += static_cast<char>(c);
                }
        }
    }
    return out;
}

// 每次都重新打开并追加。比持有 FILE* 慢，但换来两个性质：
// 进程异常终止不丢已写入的行；无需依赖静态析构顺序。
inline void emit(const std::string& json_line) {
    const char* path = std::getenv("D2X_RESULT_FILE");
    if (!path || !*path) return;                 // 学员直接运行：什么都不做

    if (std::FILE* f = std::fopen(path, "a")) {
        std::fputs(json_line.c_str(), f);
        std::fputc('\n', f);
        std::fclose(f);
    }
}

inline void assertion(bool ok, const std::string& expr,
                      const std::string& expected, const std::string& actual,
                      const char* file, int line) {
    std::string out = "{\"kind\":\"assert\",\"ok\":";
    out += ok ? "true" : "false";
    out += ",\"expr\":\""     + escape(expr)     + "\"";
    out += ",\"expected\":\"" + escape(expected) + "\"";
    out += ",\"actual\":\""   + escape(actual)   + "\"";
    out += ",\"file\":\""     + escape(file ? file : "") + "\"";
    out += ",\"line\":" + std::to_string(line) + "}";
    emit(out);
}

inline void wait(const char* file, int line) {
    std::string out = "{\"kind\":\"wait\",\"file\":\"";
    out += escape(file ? file : "");
    out += "\",\"line\":" + std::to_string(line) + "}";
    emit(out);
}

} // namespace d2x::report

#endif // REPORT_HPP_D2X
