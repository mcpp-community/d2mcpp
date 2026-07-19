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

// d2mcpp 的练习断言协议，定义在 d2x/cpp/common.hpp：
//   d2x_assert / d2x_assert_eq 失败时打印 ❌
//   D2X_WAIT 宏打印一句提示，学员删掉它才算真正完成
//
// 这套判定是「课程约定」，不是「构建工具行为」——所以它属于 Provider，
// 而不是 d2x 框架。换一门 Rust 课程，这里会完全不同。
export constexpr std::string_view kFailMark = "\xE2\x9D\x8C";  // U+274C ❌
export constexpr std::string_view kWaitMark = "D2X_WAIT";

export enum class Outcome { Pass, Fail, Blocked };

export std::string_view to_string(Outcome o) {
    switch (o) {
        case Outcome::Pass:    return "pass";
        case Outcome::Fail:    return "fail";
        case Outcome::Blocked: return "blocked";
    }
    return "fail";
}

// 运行阶段的输出判定。退出码为 0 只是必要条件：
//   出现 ❌      → 断言没过，Fail
//   出现 D2X_WAIT → 答案已对但路障还在，Blocked（既非失败也不该前进）
export Outcome judge_run(int exit_code, std::string_view output) {
    if (exit_code != 0) return Outcome::Fail;
    if (output.find(kFailMark) != std::string_view::npos) return Outcome::Fail;
    if (output.find(kWaitMark) != std::string_view::npos) return Outcome::Blocked;
    return Outcome::Pass;
}

// mcpp 需要在 workspace 根（.d2x/build/）下执行。Provider 是独立进程，
// 直接 chdir 比拼 `cd X && ...` 更干净，也避开 Windows 的 shell 差异。
export void enter(const fs::path& workspace_root) {
    fs::current_path(workspace_root);
}

export Captured build_current() {
    return capture("mcpp build -q -p _current");
}

export Captured run_current() {
    return capture("mcpp run -q -p _current");
}

} // namespace d2x::runner
