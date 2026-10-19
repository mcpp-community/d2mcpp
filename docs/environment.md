# 环境准备与排错 / Environment setup and recovery

课程版本以根目录 [`.xlings.json`](../.xlings.json) 为准：mcpp `2026.10.17.1`、d2x `2026.08.02.2`、mdBook `0.4.43`。安装器基线由 [`scripts/tool-versions.env`](../scripts/tool-versions.env) 管理。支持 Linux x86_64、macOS 14+ ARM64、Windows x86_64 / PowerShell 5.1+；当前 d2x Release 没有 Linux ARM64 资源。

The course pins its tools in `.xlings.json`. The scripts support Linux x86_64, macOS 14+ Apple Silicon, and Windows x86_64 with Windows PowerShell 5.1+. Your preferred editor and Visual Studio are optional. The d2x release used by this course does not yet ship Linux ARM64 binaries.

## 准备环境 / Prepare the environment

在课程根目录执行对应系统的命令。脚本会安装固定版本、显示首次 Provider 构建过程，并验证 Provider 可以描述自身和枚举练习。失败时立即停止；不要忽略错误继续启动 checker。

Run from the course root. Preparation installs the pinned tools, visibly builds the Provider, and checks its description and exercise discovery. The first build also downloads mcpp's private C++ toolchain and builds the standard library module; installing the mcpp executable alone does not complete this preparation.

Linux / macOS:

```bash
bash scripts/setup.sh
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
mcpp test -p solutions
d2x checker
```

Windows PowerShell:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\setup.ps1
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
mcpp test -p solutions
d2x checker
```

准备脚本可以重复执行，不覆盖练习或清理 `.d2x/state.json`。`git pull` 更新配置后，需要重新运行它。参考答案必须全部通过（当前 52 份）；未完成的练习编译或运行失败则属于正常练习过程。

Preparation is repeatable and preserves exercises and `.d2x/state.json`. Repeat it after `git pull` to apply updated pins. Reference answers must all pass (currently 52). Unfinished exercises are expected to fail.

## Codespaces / Dev Container

等待创建阶段完成依赖安装、Provider 构建和参考答案测试。只有全部成功才显示 ready。进入 VS Code 后在可见终端执行 `d2x checker`，或选择 **Tasks: Run Task → d2mcpp: practice**。这个任务限制一个实例；重连不会自动启动另一个 checker，也不会重复下载和跑全部参考答案。

Wait for creation to finish installing tools, building the Provider and testing reference answers. Then run `d2x checker` in a terminal, or use **Tasks: Run Task → d2mcpp: practice**. The task permits one instance. Reconnection only checks readiness; it does not start a background TUI or repeat the full setup.

容器配置包含 SSH Feature，也支持通过 `gh codespace ssh` 连接和 `gh codespace logs` 获取创建日志。已有 Codespace 更新这项配置后需要重建容器才能生效。

The container includes the SSH Feature for `gh codespace ssh` and creation logs via `gh codespace logs`. Rebuild an existing Codespace to apply this configuration change. See [GitHub's CLI connection instructions](https://docs.github.com/en/codespaces/developing-in-a-codespace/using-github-codespaces-with-github-cli).

如果更新配置后提示环境尚未准备好，执行：

If the configuration changed or creation failed, rerun inside the container:

```bash
bash .devcontainer/postCreate.sh
```

打开创建日志查看失败阶段。`postAttachCommand` 的输出是就绪提示，不是练习终端。普通本地安装使用 `scripts/setup.sh` / `scripts/setup.ps1`，不要执行容器专用的 apt/sudo 初始化脚本。

Use the creation log to find the failing phase. The attach log is a readiness message, not the interactive practice terminal. Outside the container, use the local preparation scripts instead of the container's apt/sudo setup.

## Provider 无响应 / Provider did not respond

这个消息表示 d2x 没有收到 Provider 的描述事件，既可能是找不到命令，也可能是下载、构建、链接或协议错误。先直接执行下列命令，把底层 stderr 保留下来：

The message means d2x received no valid Provider description. Run the underlying command directly to expose a missing command, download/build/link failure, or protocol error:

```text
xlings --version
mcpp --version
d2x --version
mcpp run -p d2x/buildtools -- describe
```

Linux/macOS 用 `command -v xlings mcpp d2x`，PowerShell 用 `Get-Command xlings,mcpp,d2x` 检查实际路径。进入课程目录后应使用项目固定版本；在目录外安装新版不一定会改变项目版本。若找不到工具，先更新当前终端 PATH（见上面的命令），或重新打开终端。

Check executable paths with `command -v xlings mcpp d2x` on Unix or `Get-Command xlings,mcpp,d2x` in PowerShell. The project pin takes precedence inside the course. Update this terminal's PATH as shown above or reopen the terminal after installation.

新 mcpp 支持以下诊断；先把版本更新成功再使用，旧版不一定提供同一命令：

After upgrading mcpp, collect its read-only diagnosis and toolchain inventory:

```text
mcpp self doctor
mcpp toolchain list --format json
```

提交问题时附系统/架构、工具版本与路径、失败的命令及原始错误即可。不要附完整 `.d2x.json`，它可能包含 API key。

When reporting an issue, include OS/architecture, versions/paths, the failing command and its original error. Do not upload your whole `.d2x.json`; it may contain an API key.

## `/nonexistent/.../libm.so.6` / `libmvec.so.1`

这类链接错误表明 glibc 载荷的打包占位路径没有被正确迁移，Provider 尚未生成。先更新课程并重新运行准备脚本，确认 `mcpp --version` 是 `2026.10.17.1`。不要手工创建 `/nonexistent` 目录或修改练习来规避环境错误。

These linker errors mean the packaged glibc paths were not relocated correctly; the Provider has not been built. Update the course, rerun preparation and confirm `mcpp --version` reports `2026.10.17.1`. Keep your exercises and learning progress.

`xlings update` 刷新课程工具的索引；`mcpp index update` 刷新 mcpp 内部工具链索引。二者不是一回事，刷新索引也不等于修复已经损坏的载荷。`mcpp toolchain install <family> <version>` 可用于按诊断结果更新指定工具链，但不能保证修复所有遗留文件；如果重新准备仍失败，可以使用下面可回退的新 home 恢复方式。

`xlings update` and `mcpp index update` refresh different package layers. An index refresh does not itself repair damaged installed files. A targeted `mcpp toolchain install <family> <version>` can update a selected toolchain, but may not repair every legacy file. If preparation still fails, isolate a new mcpp home as below.

### 保留旧工具链，使用新 home / Recover with a fresh home

此操作保留原有 home、课程代码和学习进度，但需要重新下载一份工具链，占用额外磁盘空间。先从 `mcpp self doctor` 记录原 home。选择一个尚未使用的路径，再执行对应系统的命令；后续练习会话也需要设置同一个 `MCPP_HOME`。

This keeps the previous home, course and progress intact, at the cost of another toolchain download. Record the original home from `mcpp self doctor`. Choose an unused path; set the same `MCPP_HOME` in subsequent practice sessions.

```bash
# Linux / macOS
export MCPP_HOME="$HOME/.mcpp-d2mcpp-recovered"
bash scripts/setup.sh
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
mcpp test -p solutions
d2x checker
```

```powershell
# Windows PowerShell
$env:MCPP_HOME = "$env:USERPROFILE\.mcpp-d2mcpp-recovered"
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\setup.ps1
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
mcpp test -p solutions
d2x checker
```

撤销当前会话的覆盖：Unix 使用 `unset MCPP_HOME`，PowerShell 使用 `Remove-Item Env:MCPP_HOME`。原 home 仍然保留供诊断；不要自动删除整个 `~/.mcpp`、`~/.xlings` 或课程 `.d2x`。

To undo this session's override, use `unset MCPP_HOME` on Unix or `Remove-Item Env:MCPP_HOME` in PowerShell. The previous home remains available for diagnosis. No full home or progress deletion is required.

## CI 覆盖 / CI coverage

- `fresh-install.yml`：无缓存的首次检查、失败后保存、通过后推进、重启恢复；Windows 使用 PowerShell 5.1，Linux 使用无预装编译器的普通用户容器，macOS 检查 ARM64 架构。
- `dslings-ref-ci.yml`：中英文练习初始失败、答案覆盖后全部通过，以及 checker 完成全课；无 MSVC 档检查 GNU target。
- `environment-lifecycle.yml`：真实 Dev Container 配置的创建/重连，以及旧 mcpp home 升级，以及损坏 glibc、旧索引快照、悬空安装记录的恢复。
- main/定时任务额外验证真实 `d2x install d2mcpp` 下载入口；PR 的 clone 路线验证候选提交，二者分别报告。

CI logs distinguish dependency installation, initial toolchain preparation, Provider events, reference answers and checker progression. GitHub Server runners do not prove compatibility with every Windows desktop release; a CLI container test also does not replace checking the visible terminal in a real Codespace.
