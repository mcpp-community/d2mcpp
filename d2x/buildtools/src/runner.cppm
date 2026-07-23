// 调 `mcpp test --message-format json` 验证单个练习，并合并侧信道判定。
//
// 这一层不再生成任何清单、不再自己算退出码语义 —— 编译/运行的事实由
// mcpp 的 JSON 记录提供（status/exit_code/compile_output/run_output），
// 运行期语义（哪条断言挂了、路障拆没拆）由 harness 的侧信道 v2 提供，
// 两个来源在这里合并成 Provider 协议的 verdict。
module;

#include <cstdio>
#ifndef _WIN32
#  include <sys/wait.h>
#endif
#include <stdlib.h>   // setenv / _putenv_s

export module d2x.provider.runner;

import std;

namespace fs = std::filesystem;

namespace d2x::runner {

export struct Captured {
    int         exit_code{};
    std::string output;
};

// 只要 stdout：JSON 协议流在 stdout，stderr 的人读错误信息这里不需要
// （包级失败在 stdout 也有 {"error":"package"} 记录）。
//
// 注：旧实现这里要先 unsetenv("LD_LIBRARY_PATH") 绕嵌套 mcpp 的 glibc
// 段错误 —— mcpp 已在上游根治（merged_environ 剥离私有 glibc 条目），
// workaround 随之删除。
export Captured capture_stdout(const std::string& cmd) {
    Captured result;
    std::string full = cmd + " 2>/dev/null";

#ifdef _WIN32
    FILE* pipe = ::_popen(full.c_str(), "r");
#else
    FILE* pipe = ::popen(full.c_str(), "r");
#endif
    if (!pipe) return {127, std::format("failed to spawn: {}", cmd)};

    char buf[4096];
    while (std::fgets(buf, sizeof(buf), pipe)) result.output += buf;

#ifdef _WIN32
    int status = ::_pclose(pipe);
    result.exit_code = status;
#else
    int status = ::pclose(pipe);
    // pclose 回的是 wait status，转成真实退出码，否则 exit 1 会变成 256
    result.exit_code = (status == -1)          ? 127
                     : WIFEXITED(status)       ? WEXITSTATUS(status)
                     : WIFSIGNALED(status)     ? 128 + WTERMSIG(status)
                                               : status;
#endif
    return result;
}

// mcpp 的一条 per-test JSON 记录（我们关心的子集）。
export struct TestRecord {
    std::string test;             // 相对 tests/ 的路径名，如 00-auto-and-decltype/0
    std::string status;           // pass | compile_fail | run_fail
    int         exit_code{};
    std::string compile_output;
    std::string run_output;
};

export struct McppTestResult {
    std::optional<TestRecord> record;         // 精确匹配 test 名的那条
    std::string               package_error;  // {"error":"package"} 的 compile_output
    bool                      saw_any = false;
};

// 从一行 JSON 里取字段。格式由 mcpp --message-format json 产出：
// 字段固定、无嵌套对象（summary 行不取），不引入 JSON 库。
std::string field(std::string_view line, std::string_view key) {
    auto pat = std::format("\"{}\":", key);
    auto at = line.find(pat);
    if (at == std::string_view::npos) return {};
    at += pat.size();
    if (at >= line.size()) return {};

    if (line[at] == '"') {                       // 字符串值
        ++at;
        std::string out;
        while (at < line.size() && line[at] != '"') {
            if (line[at] == '\\' && at + 1 < line.size()) {
                ++at;
                switch (line[at]) {
                    case 'n': out += '\n'; break;
                    case 't': out += '\t'; break;
                    case 'r': out += '\r'; break;
                    case 'u': {                  // \u00XX —— mcpp 只对 <0x20 编码
                        if (at + 4 < line.size()) {
                            int v = 0;
                            auto hex = line.substr(at + 1, 4);
                            std::from_chars(hex.data(), hex.data() + 4, v, 16);
                            out += static_cast<char>(v);
                            at += 4;
                        }
                        break;
                    }
                    default:  out += line[at];
                }
            } else {
                out += line[at];
            }
            ++at;
        }
        return out;
    }
    auto end = line.find_first_of(",}", at);     // 裸值（数字/布尔/null）
    return std::string(line.substr(at, end == std::string_view::npos ? end : end - at));
}

// 跑 `mcpp test` 并抽出目标测试的记录。pattern 是子串匹配，可能带出
// 邻居测试（如 …/1 匹配 …/10），所以逐行解析后按 test 名精确挑。
export McppTestResult run_mcpp_test(const std::string& member,
                                    const std::string& test_name,
                                    const fs::path&    result_file) {
    std::error_code ec;
    fs::remove(result_file, ec);   // harness 是追加写的，清掉上一轮残留
    fs::create_directories(result_file.parent_path(), ec);
#ifndef _WIN32
    ::setenv("D2X_RESULT_FILE", result_file.string().c_str(), 1);
#else
    ::_putenv_s("D2X_RESULT_FILE", result_file.string().c_str());
#endif

    auto cmd = std::format("mcpp test -q -p {} {} --message-format json",
                           member, test_name);
    auto cap = capture_stdout(cmd);

    McppTestResult out;
    std::istringstream lines(cap.output);
    for (std::string line; std::getline(lines, line); ) {
        if (line.empty() || line.front() != '{') continue;
        if (line.find("\"error\":\"package\"") != std::string::npos) {
            out.package_error = field(line, "compile_output");
            out.saw_any = true;
            continue;
        }
        auto name = field(line, "test");
        if (name.empty()) continue;              // summary 行等
        out.saw_any = true;
        if (name != test_name) continue;
        TestRecord rec;
        rec.test           = name;
        rec.status         = field(line, "status");
        rec.compile_output = field(line, "compile_output");
        rec.run_output     = field(line, "run_output");
        auto raw = field(line, "exit_code");
        std::from_chars(raw.data(), raw.data() + raw.size(), rec.exit_code);
        out.record = std::move(rec);
    }
    return out;
}

// —— 侧信道判定（语义与设计稿 §5 的顺序表一致）——

export enum class Outcome { Pass, Fail, Blocked };

export std::string_view to_string(Outcome o) {
    switch (o) {
        case Outcome::Pass:    return "pass";
        case Outcome::Fail:    return "fail";
        case Outcome::Blocked: return "blocked";
    }
    return "fail";
}

export struct Failure {
    std::string what;
    std::string expected;
    std::string actual;
    std::string file;
    int         line{};
};

export struct RunReport {
    Outcome              outcome{Outcome::Pass};
    std::vector<Failure> failures;
};

// 判定顺序（与代码一致；wait 必须先于退出码判定——d2x 库在存在未拆除的
// wait 时会将退出码置为 1，若先判退出码，blocked 会被整体误判为 fail）：
//   有 ok:false            → Fail，每条失败都能转成一个 Diagnostic
//   无失败、有 wait        → Blocked（答案已对，只差拆路障）
//   无失败但退出码非 0     → Fail（纯崩溃 / 练习自己 return 非 0）
//   侧信道文件不存在        → 退回「退出码为 0 即通过」
//
// 最后一条让 harness 自动变成可选的：纯观察型练习可以是零依赖的
// 纯 C++ 文件，学习者能原样拷进 Compiler Explorer。
export RunReport judge_run(int exit_code, const fs::path& result_file) {
    RunReport report;

    std::ifstream in(result_file);
    if (!in) {
        report.outcome = (exit_code == 0) ? Outcome::Pass : Outcome::Fail;
        return report;
    }

    bool saw_wait = false;
    for (std::string line; std::getline(in, line); ) {
        auto kind = field(line, "kind");
        if (kind == "wait") { saw_wait = true; continue; }
        if (kind != "assert") continue;
        if (field(line, "ok") == "true") continue;

        int line_no = 0;
        auto raw = field(line, "line");
        std::from_chars(raw.data(), raw.data() + raw.size(), line_no);

        report.failures.push_back(Failure{
            .what     = field(line, "what"),
            .expected = field(line, "expected"),
            .actual   = field(line, "actual"),
            .file     = field(line, "file"),
            .line     = line_no,
        });
    }

    if (!report.failures.empty()) report.outcome = Outcome::Fail;
    else if (saw_wait)            report.outcome = Outcome::Blocked;
    else if (exit_code != 0)      report.outcome = Outcome::Fail;
    else                          report.outcome = Outcome::Pass;

    return report;
}

} // namespace d2x::runner
