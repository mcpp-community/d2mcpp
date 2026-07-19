// 调用 mcpp 构建/运行单个练习，并判定结果。
module;

// popen/pclose 是 POSIX，WIFEXITED 等是宏——都不在 import std 的范围内。
#include <cstdio>
#ifndef _WIN32
#  include <sys/wait.h>
#  include <stdlib.h>   // unsetenv
#endif

export module d2x.provider.runner;

import std;

namespace fs = std::filesystem;

namespace d2x::runner {

export struct Captured {
    int         exit_code{};
    std::string output;
};

// 合并 stderr —— 编译错误和练习的运行输出都要原样呈现给学员。
// 这些内容最终进 JSON 的 output 字段，不会污染协议。
//
// 必须先清掉 LD_LIBRARY_PATH：本 Provider 由 `mcpp run` 启动时，mcpp 会把
// LD_LIBRARY_PATH 指向它私有的 glibc（~/.mcpp/registry/.../xim-x-glibc/*/lib64）
// 并注入子进程。我们接着去 spawn 嵌套的 mcpp —— 那是另一个二进制，被迫加载
// 错配的 glibc 后会在动态链接器里段错误。清空后 mcpp 会为它自己的子进程重新
// 设置正确的值，所以练习程序照常能跑。
//
// d2x 侧对同一问题有相同的处理（platform.cppm 的 run_command_capture）。
export Captured capture(const std::string& cmd) {
    Captured result;
#ifndef _WIN32
    ::unsetenv("LD_LIBRARY_PATH");
#endif
    std::string full = cmd + " 2>&1";

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

// 这套判定是「课程约定」，不是「构建工具行为」——所以它属于 Provider，
// 而不是 d2x 框架。换一门 Rust 课程，这里会完全不同。
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
    std::string expr;
    std::string expected;
    std::string actual;
    std::string file;
    int         line{};
};

export struct RunReport {
    Outcome              outcome{Outcome::Pass};
    std::vector<Failure> failures;
};

// 从侧信道 NDJSON 里取一个字段。这里不引入 JSON 库：
// 格式由我们自己的 harness 产出，字段固定、无嵌套、无数组。
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
                    default:  out += line[at];
                }
            } else {
                out += line[at];
            }
            ++at;
        }
        return out;
    }
    auto end = line.find_first_of(",}", at);     // 裸值（数字/布尔）
    return std::string(line.substr(at, end == std::string_view::npos ? end : end - at));
}

// 判定顺序：
//   有 ok:false            → Fail，每条失败都能转成一个 Diagnostic
//   无失败但有 wait        → Blocked（答案已对，只差拆路障）
//   侧信道文件不存在        → 退回「退出码为 0 即通过」
//
// 最后一条让 harness 自动变成可选的：纯观察型练习可以是零依赖的
// 纯 C++ 文件，学员能原样拷进 Compiler Explorer。
export RunReport judge_run(int exit_code, const fs::path& result_file) {
    RunReport report;

    if (exit_code != 0) {
        report.outcome = Outcome::Fail;
        // 即使退出码非 0，已写入的断言仍然有价值（崩溃前过了几条）
    }

    std::ifstream in(result_file);
    if (!in) {
        // 没有侧信道：练习没用 harness，退出码就是全部信息
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
            .expr     = field(line, "expr"),
            .expected = field(line, "expected"),
            .actual   = field(line, "actual"),
            .file     = field(line, "file"),
            .line     = line_no,
        });
    }

    if (!report.failures.empty()) report.outcome = Outcome::Fail;
    else if (exit_code != 0)      report.outcome = Outcome::Fail;
    else if (saw_wait)            report.outcome = Outcome::Blocked;
    else                          report.outcome = Outcome::Pass;

    return report;
}

// mcpp 需要在 workspace 根（.d2x/build/）下执行。Provider 是独立进程，
// 直接 chdir 比拼 `cd X && ...` 更干净，也避开 Windows 的 shell 差异。
export void enter(const fs::path& workspace_root) {
    fs::current_path(workspace_root);
}

export Captured build_current() {
    return capture("mcpp build -q -p _current");
}

// 运行前把侧信道路径告诉 harness，并清掉上一轮的残留 ——
// harness 是追加写的，不清会把上次的失败算进这次。
export Captured run_current(const fs::path& result_file) {
    std::error_code ec;
    fs::remove(result_file, ec);
#ifndef _WIN32
    ::setenv("D2X_RESULT_FILE", result_file.string().c_str(), 1);
#else
    ::_putenv_s("D2X_RESULT_FILE", result_file.string().c_str());
#endif
    return capture("mcpp run -q -p _current");
}

} // namespace d2x::runner
