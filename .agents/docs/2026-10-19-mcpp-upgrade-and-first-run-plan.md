# mcpp 2026.10.17.1 升级与首次使用体验优化方案

日期：2026-10-19。状态：已进入实现，[draft PR #91](https://github.com/mcpp-community/d2mcpp/pull/91)。第 8 节保留调研时记录；第 9 节说明实现与验收状态。最终 CI 以 PR 当前提交的检查结果为准。

## 1. 建议与范围

将根 `.xlings.json` 的 mcpp 升级为统一字符串 `2026.10.17.1`，同时修复初始化失败处理，增加独立的首次使用 CI，并同步本地、云端和三种语言的说明。旧环境升级单独验收，避免新建环境通过、已有 Codespaces 仍然不可用。

版本配置建议：

```json
{
  "workspace": {
    "d2x": "2026.08.02.2",
    "mdbook": "0.4.43",
    "mcpp": "2026.10.17.1"
  }
}
```

移除必装的 `code: ""`，把编辑器选择放进可选说明。Codespaces 已提供 VS Code；本地课程运行也不应要求下载桌面编辑器。保留 mdBook 以支持现有 `d2x book`。d2x 暂不升级：其最新 Release 仍是 `2026.08.02.2`。外层 xlings 安装器建议统一固定为本次调研时最新的 `2026.10.18.1`，经矩阵验收后采用；mcpp 自带的 xlings 由 mcpp Release 管理，不能把这两个安装层混为一谈。

已核实：[mcpp Release](https://github.com/mcpp-community/mcpp/releases/tag/v2026.10.17.1) 与 [mcpp 包索引](https://github.com/openxlings/xim-pkgindex/blob/main/pkgs/m/mcpp.lua) 均已发布目标版本。[xlings 项目说明](https://github.com/openxlings/xlings/blob/v2026.10.18.1/docs/04-projects.md) 明确支持 `"pkg": "ver"` 表示跨平台共用版本。平台与架构的资源选择交给包索引。

## 2. 故障分析：已确认与待验证

用户提供的直接运行日志：

```text
mcpp run -q -p d2x/buildtools --
error: build failed
failed: bin/d2x-buildtools
.../ld: cannot find /nonexistent/xlings-use-rpath-not-default-search/padding-to-255-bytes-for-install-time-relocation.../lib/libm.so.6
.../ld: cannot find .../lib/libmvec.so.1
collect2: error: ld returned 1 exit status
```

**直接原因已经确认是 Provider 链接失败。** 链接阶段引用了打包时保留的占位路径，Provider 未生成，无法返回 `describe`，上层才显示“Provider 无响应”。这不是从现有日志能够判定的协议实现错误，也不是练习答案问题。

[glibc 包配方](https://github.com/openxlings/xim-pkgindex/blob/main/pkgs/g/glibc.lua) 使用同名占位前缀，并对链接脚本与二进制执行安装时路径迁移。配方明确指出旧客户端缺少 `elfpatch.relocate_build_paths` 时只做旧路径回退。因此，“旧 mcpp 内置包管理组件与新 glibc 载荷/配方之间的迁移兼容问题”是有源码依据的解释；具体是哪份 glibc 版本、revision、配方与安装状态，仍需故障 Codespace 的诊断输出确认。不能只凭发布说明断言升级必然修复这份已损坏的安装。

仓库内确定存在的放大因素：

| 位置 | 当前行为 | 影响 |
|---|---|---|
| `.xlings.json` | 三个平台重复固定 `2026.8.2.1` | 新安装仍使用八月版 mcpp；全局装最新版也可能被项目版本覆盖 |
| `.devcontainer/postCreate.sh` | xlings 固定 `0.4.68`，仅在命令不存在时安装 | 新旧环境的安装器版本不一致 |
| 同上 | Provider 预热失败后 `echo warning`，仍打印 ready | 环境创建看似成功，首次 checker 才暴露构建错误 |
| `.devcontainer/devcontainer.json` | `postAttachCommand` 直接执行长期运行的 checker | 没有就绪检查，也不能保证学习者在可交互终端看到练习界面 |
| 现有 CI | xlings Unix 固定 `0.4.51`；Windows 脚本当前支持读取同一环境变量 | CI 与 Codespaces 版本不一致；Windows 是否安装该版本应以输出断言，不能仅凭没有位置参数认定其未固定 |
| 现有 CI | 先 build Provider，再 solutions/e2e，最后 checker | 首次 checker 已被预热，不能证明冷启动可用 |
| workflow paths | 未包含 `.devcontainer/**`、README、入门章节 | 环境和操作说明变化可能完全不触发验证 |
| README | 两个获取入口、首次环境验证、直接 checker | 需要分别复现用户命令顺序，不能用预构建后的测试替代 |

还存在独立的冷启动风险：d2x 默认 Provider 空闲超时为 120 秒，而 `mcpp run -q` 首次构建 Provider 时，Provider 本体还没启动，其 `check` 心跳无从生效。实际是否超过 120 秒要在干净环境测量；用户此次日志已明确是链接错误，不能把它归为超时。

## 3. 初始化和进入练习

### 本地准备步骤

两条获取路线都收敛到相同的课程目录准备步骤：

```text
安装/检查受验收的 xlings
→ 获取课程（d2x install 或 git clone）
→ xlings update + xlings install -y
→ 确认实际 mcpp/d2x 版本与执行路径
→ mcpp build -p d2x/buildtools（显示构建过程）
→ Provider describe / exercises 自检
→ mcpp test -p solutions
→ d2x checker
```

新增 `scripts/setup.sh` 与 `scripts/setup.ps1`：执行项目依赖安装、版本检查、Provider 构建和自检；任一步失败立即退出并保留原始错误。Windows 用 PowerShell 原生执行，不要求学习者另装 Git Bash、Python 或 GNU timeout。两端脚本保持相同阶段，CI 验证输出契约，避免引入新的跨平台运行时依赖。完整 solutions 验证保留为明确步骤，便于观察耗时和定位问题。

初始化本身不进入无限练习循环；它成功后才提示 `d2x checker`。重复运行准备步骤应成功，且不修改练习、参考答案和学习进度。直接执行 checker 的冷启动另设测试；若静默构建超过空闲时限，按实测给课程设置有限的 `provider_idle_timeout`，不能简单关闭超时作为验收条件。

### Codespaces / Dev Container

`postCreate.sh` 调用上述准备逻辑，保留必要系统依赖安装；删除预热失败后继续 ready 的分支。先把安装器加入当前脚本 PATH，再判断和校验版本，避免“已经安装但当前 shell 找不到”。安装器版本集中存放一处，CI、容器脚本及文档从同一配置读取，避免继续维护三套值。

生命周期建议：

1. `postCreateCommand` 完成安装、Provider 验证和参考答案环境验证，非零退出即创建失败；输出明确阶段。
2. 设置 `waitFor: "postCreateCommand"`，让工作区可用时已经完成准备。
3. 使用 VS Code 工作区任务在可交互终端启动 `d2x checker`，工作目录固定为 `${workspaceFolder}`。任务单实例；自动任务获准后可自动启动，首次授权提示存在时仍提供清晰的手动入口。避免让 `postAttachCommand` 承载永久 TUI 进程。
4. attach/reconnect 只检查就绪并显示启动入口，不重复下载、跑全部参考答案或创建第二个 checker。使用按配置内容识别的就绪标记；变更 pin 后旧标记失效，失败时不写标记。
5. README 对自动启动的描述必须与实际终端行为相符；若最终选择手动启动，三种语言同时改为“准备完成后运行 d2x checker”。

容器生命周期语义依据 [Dev Container 规范](https://containers.dev/implementors/json_reference/)。自动任务的真实交互行为须在新建 Codespace 中人工验收，不能由容器内一条 CLI 命令推定。

## 4. CI 设计：分开验证三类状态

### A. fresh-install：用户首次使用

新增 `.github/workflows/fresh-install.yml`，不恢复 mcpp/xlings/build/state 缓存。各分支使用独立安装目录及全新课程副本；开始时断言无 Provider 产物、无学习进度，记录系统、架构、shell、工具版本与实际路径。

| 环境 | 主要覆盖 |
|---|---|
| Ubuntu 24.04 x86_64，普通用户的最小容器 | 无预装 C++ 工具链，覆盖 Linux 链接与载荷安装 |
| macOS ARM64 托管 runner | 发布资源架构、SDK、首次安装；实际架构必须断言 |
| Windows Server 2022 x64 / 2025 x64，PowerShell | 本地文档默认 shell、安装器 PATH、首次使用 |
| Windows 2025 x64，遮蔽 MSVC | 验证无 Visual Studio 时的 MinGW 回退，沿用并检查遮蔽后置条件 |

mcpp 当前 macOS Release 提供 ARM64；不要把相同版本字符串误解为任意架构均受支持。runner 标签及架构按 [GitHub 官方说明](https://docs.github.com/en/actions/reference/runners/github-hosted-runners) 选择，并在运行时核验。Server runner 也不能作为已测试真实 Windows 10/11 桌面的依据。

同一平台分开测试，禁止共享预热结果：

- **README 路线**：在空目录获取课程，按文档顺序安装、准备、全量参考答案验证，再启动 checker。
- **直接 checker 路线**：另一份空构建/空缓存环境，依赖安装完成后立即 `d2x checker --ui print --emit-events`；此前不能执行 mcpp build、describe、solutions 或 d2x status。验证 Provider 启动及第一题正常判定，测量首次响应时长。

clone 路线在 PR 验证当前提交；真实 `d2x install d2mcpp` 路线会通过课程索引获取已发布/main 的课程，不能冒充对 PR 的验证。PR 若要覆盖该下载链，应另配置指向候选提交的临时课程索引；如果暂不做此适配，明确让定时/main job 验证真实安装入口，并校验下载课程的 `.xlings.json` 与预期版本。测试需要把“安装命令成功但课程快照过期”作为失败。

新增 CI 检查器 `scripts/ci/first-run.py`：Python 仅作为 CI 驱动，跨平台用子进程与 NDJSON 事件检查状态，不依赖 grep 彩色界面文本。

必须断言：

1. `describe` 返回 protocol 1、name mcpp；练习 ID 非空且唯一，数量与课程源码一致（当前每种语言 52）。
2. pristine 的 `hello-mcpp` 返回预期编译失败判定并进入等待编辑状态；包级构建错误、无 verdict、无练习、Provider 无响应均失败。不能把“进程存活”视为成功。
3. 在独立课程副本中用对应参考答案替换首题，checker 感知文件保存并判定通过、进入下一题。这覆盖 watcher；现有全部答案预先覆盖的脚本没有验证运行中保存。
4. 停止后再次启动能够读取已完成进度；重复 setup 成功、进度不丢失。
5. solutions 全部通过且实际数量等于答案文件数；失败不能只保留一句 `build failed`。

不要仅检查 d2x 退出码：固定版本的 checker 初始化错误分支会记录错误并返回，未必将失败变为进程非零状态。NDJSON 与预期状态是必要判据。TUI 另保留一次终端人工检查。

### B. exercises-and-checker：现有回归

保留 `dslings-ref-ci.yml` 的 pristine / overlay / solutions / checker 全程完成验证；统一安装器版本和 pin 注释。升级后检查测试输出兼容性、JSON 字段及产物布局。尤其不要用旧的 `d2x/buildtools/target/x86_64-windows-gnu` 路径作为唯一回退判据；先依据新版本实际构建元数据/工具链诊断确认选中 GNU target，再对照产物验证。

保持 overlay passed 等于覆盖数量的既有断言；调研草稿曾误记此处仅拒绝零通过，实施复核后已更正。加强 pristine 计数，确认每个预期测试确实被发现，不能把包级失败或零测试当作“所有练习未完成”。

### C. devcontainer 与 upgrade

- **devcontainer job**：用 Dev Container CLI 从仓库真实配置创建环境，执行真实 `postCreateCommand`，随后由新 shell 验证版本、describe/exercises、solutions 及首题编辑推进；不能以普通 Ubuntu runner 替代。覆盖二次 attach，验证 PATH 和重复启动行为。容器内测试成功仍需一次真实 Codespaces 新建的人工验收。
- **upgrade job**：先用旧 pin 初始化或重现故障，保留该 mcpp home/registry，升级外层 xlings 与项目 mcpp 后重跑 Provider、solutions、checker。分别验证旧缓存、旧索引/载荷及项目内 `.xlings`；保存旧版建环境失败的日志，不能因旧版建环境失败就跳过后续迁移。
- 在 Linux 额外验证长路径/带空格路径；Windows 验证 PowerShell 5.1 兼容性或在 README 明确最低 PowerShell 版本。macOS 不依赖预装 GNU timeout。

PR/main 触发范围覆盖 `.xlings.json`、`.d2x.json`、`mcpp.toml`、`d2x/**`、`src/**`、`solutions/**`、`.devcontainer/**`、`scripts/**`、相关 workflow、三种 README 与中英文环境安装章节。增加定时运行，发现外部包索引/安装器变化导致的回归；静态 CI 配置检查和 Markdown 链接检查在文档变更时执行。

失败归档安装/构建 stderr、Provider NDJSON、checker 事件、实际版本/路径、mcpp 工具链诊断、所用索引 revision 与计数。日志不包含 API key 或整个用户配置。网络下载可有限重试；构建与协议断言失败不能无限重试。

## 5. 已有故障环境的修复说明

维护文档先指导收集 `mcpp --version`、`xlings --version`、实际执行路径、直接运行 `describe` 的 stderr；新 mcpp 可进一步运行只读的 `mcpp self doctor`。旧版本不一定支持同一诊断命令。

推荐修复顺序：

1. 保留练习与 `.d2x` 进度，更新课程配置和受验收的外层 xlings；运行项目依赖安装。
2. **确认目录内实际 mcpp 是 `2026.10.17.1`**，再重新构建 Provider 并验证 describe/exercises。
3. 若仍有旧占位路径错误，查明实际 mcpp home 与所选工具链，用新版支持的 `mcpp toolchain install <实际工具链>` 更新其安装载荷，再复验。`mcpp index update` 与外层 `xlings update` 是不同层的操作，单独刷新索引不等于修复已安装文件。
4. 若定向修复仍失败，提供可回退的“保留旧 home，改用新工具链 home”恢复步骤，并明确空间开销；先验证新 home 独立可用，再决定如何处理旧目录。不在脚本中自动删除用户全部 `~/.xlings` 或 `~/.mcpp`。

上述定向修复命令必须在旧环境升级 job 中跑通后才进入用户快速开始。新版发布说明的修复范围包括安装记录、载荷 revision 与诊断，不能替代本项目对八月版遗留状态的迁移实测。

## 6. 文档与文件改动清单

| 文件 | 调整内容 |
|---|---|
| `.xlings.json` | mcpp 单字符串 pin；编辑器改可选 |
| `scripts/` 的版本配置、setup.sh/setup.ps1 | 统一安装器版本、准备流程、错误处理与可重复执行 |
| `.devcontainer/postCreate.sh` | 调用公共准备流程；构建失败立即终止；通过验证后才 ready |
| `.devcontainer/devcontainer.json`、VS Code 任务配置 | 等待准备完成；可见终端进入 checker；重连不重复启动 |
| `.github/workflows/fresh-install.yml` | 全新安装、文档路线与直接 checker 路线 |
| `.github/workflows/dslings-ref-ci.yml` | 复用版本、扩大触发路径、更新目标判据与计数 |
| Dev Container / upgrade workflow | 真实生命周期测试与旧工具链迁移 |
| `scripts/ci/first-run.py` | Provider、首次检查、保存推进与恢复进度的行为断言 |
| `README.md`、`README.zh.md`、`README.zh.hant.md` | 版本、重开终端/PATH、两个获取路线、准备/验证/练习命令、云端预期与故障入口同步 |
| `book/src/base/chapter_1.md`、`book/en/src/base/chapter_1.md` | 与 README 共用同一流程，区分练习预期失败与环境故障；实施时遵循 d2mcpp-authoring skill |
| 新的环境故障排查文档 | Provider 失败诊断、占位路径案例、旧环境恢复、平台/架构范围 |

三种 README 的代码链接仍指向已移除的 `dslings/`，应同步指向 `src/`。英文 README 的更多细节链接应指向英文入门页。更新工具链后，`git pull` 说明也要提示重新运行项目准备；拉取代码不等于安装新 pin。

## 7. 交付顺序与完成标准

1. **P0**：统一 mcpp pin、安装器基线，修复容器吞错，验证 Provider 与 solutions；先解除新用户无法启动的问题。
2. **P1**：fresh-install、Dev Container 和 upgrade job，补首次失败→保存→推进→恢复进度；所有平台都执行文档流程。
3. **P1**：三种 README、双语入门章节与排错文档同步，替换失效链接；实现可见的 Codespaces 练习入口。
4. **P2**：根据实测优化下载与预构建。预构建改善等待体验，但保留完全无缓存的测试路径。

合并前要求：目标平台矩阵通过；全量参考答案数量正确；首次 checker 抵达预期练习状态且保存后推进；已有故障环境可恢复；真实 Codespace 从按钮创建到可见终端练习人工通过。每一步均有日志证据，不能将“安装成功”或“CI 已有类似测试”替代端到端验收。

## 8. 调研阶段验证记录

- 已读取仓库配置、两份 E2E 脚本、Provider、三种 README 与双语入门流程，并核对 mcpp/xlings/d2x 发布及包索引。
- 已对照用户的实际链接错误定位占位路径来源，并检查固定版本 d2x 的默认超时、describe 和初始化失败分支。
- Ubuntu 24.04 隔离测试在 Docker Hub 拉镜像时网络超时，尚未执行课程安装；因此不能宣称已完成 Codespaces 等价环境验证。
- 使用本地已有的 Ubuntu 20.04 基础镜像补充隔离探测：创建普通用户及全新 home，从提交 `6bcf5db` 导出的课程副本，仅把 mcpp 改为目标字符串并移除必装 code；安装外层 xlings `2026.10.18.1` 后执行 `xlings update`、`xlings install -y`。依赖安装成功，实际版本符合预期；Provider describe 返回 protocol 1，exercises 返回 52 条；`mcpp test -p solutions` 为 52 passed / 0 failed。此测试没有使用原工作区的缓存或改动练习，也没有执行真实 Dev Container 生命周期。
- 此环境的首次构建包含私有 registry 初始化、索引下载、patchelf/ninja 和 GCC 安装，说明仅安装 mcpp 包并不代表首次 C++ 构建已就绪。mdBook 单个下载约 116 秒，另提示应把下载时间与编译时间分别记录。
- 同一隔离副本继续运行现有 `checker-e2e.sh zh`：checker 自主完成 52/52 道练习并正常退出，容器测试进程退出码 0。该项使用预先覆盖的答案，尚不覆盖运行中编辑保存、直接 checker 冷启动、TUI 和旧 home 迁移。完整探测日志保留于执行机器的 `/tmp/d2mcpp-upgrade-research/linux-fresh-ubuntu20.log`，临时容器已自动移除。
- Windows、macOS 和真实 Codespaces 尚未执行；这些是实施验收项。

## 9. 实现与验收记录

- 已实现统一 mcpp pin、可选编辑器、共享安装器版本、Shell/PowerShell 5.1 准备流程及失败即停止的容器初始化。Codespaces 选择明确的手动启动：准备完成后在可见终端执行 `d2x checker`，或启动单实例 VS Code 任务，重连只验证就绪。
- 三种 README、双语入门章节、双语环境排错说明与双语更新日志同步。PR 构建两本电子书并检查本地文档链接。
- 首次使用检查器已在本地实际跑通失败→保存→通过→推进→重启，以及重复 setup 保留进度。额外注入“d2x 空输出并退出 0”的故障，检查器正确拒绝，防止仅凭退出码误报成功。
- CI 已覆盖最小 Linux 普通用户、macOS ARM64、Windows Server 2022/2025（有/无 MSVC）的 README 与直接 checker 两条路径。升级矩阵保留真实八月版 home，并注入损坏 glibc、八月包索引快照、悬空安装记录三个场景。
- 已有 CI 证据：Linux/macOS 两条首次使用路线、实际 Dev Container 创建/重连、Linux/macOS 旧 home 升级及 Linux 三个故障状态均曾通过；旧索引路线也在本地独立通过。由于后续修复了 Windows 安装与测试输出兼容问题，仍需以 PR 最新提交的完整矩阵结果验收。
- Windows PowerShell 5.1 实测发现固定版本上游安装器的受限 Range 请求头，以及 SHA256 下载响应为字节数组的问题。课程引导脚本直接下载固定 Release、从文件读取 sidecar 并校验，再调用 `self install`；README 的 Windows 可选下载入口同步使用该引导脚本。测试输出显式使用 UTF-8，避免 CP1252 使成功构建后的日志打印崩溃。
- 无 MSVC 档允许 doctor 报预期警告，但必须证明实际选中 gcc、对应 MinGW 载荷和 GNU 产物；不以忽略所有诊断代替回退验证。
- 真实 `d2x install d2mcpp` 入口放在 main/定时任务，因为 PR 阶段该入口仍下载 main；此项不冒充候选提交的验证。当前没有合并或发布主分支。
- 用户已补充 Codespaces 授权。首次真实创建发现自定义 C++ 镜像不包含 SSH 服务，CLI 连接和日志获取失败；配置补充官方 SSH Feature，并同步连接/重建文档。真实终端验收与最终 CI 结果记录在 PR，Dev Container CI 不代替该交互验收。
