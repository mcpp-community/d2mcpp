# d2x 与 d2mcpp 最新版联动及首次启动优化方案

日期：2026-10-20。状态：待 review，仅提交方案；未修改运行代码、版本 pin 或包索引。

## 1. 建议与边界

推荐同时解决三个问题：实际运行最新版、Provider 首次构建被误杀、失败缺少可定位的日志。d2x 负责通用的启动期限、进度呈现、进程清理和错误传播；d2mcpp 负责课程版本、构建入口和真实用户流程验收；包索引负责发布后正确解析 latest。

“使用最新版”采用两个配合的入口：用户公开安装入口跟随最新稳定版；课程配置固定当次最新且已通过联动验收的具体版本。定时 CI 检查上游稳定版，有更新即推动升级，并报告版本落后，不能长期停在旧版本。PR 验收固定具体候选版本，使结果可复现。不要在运行 checker 时隐式联网升级或重装依赖。

当前 GitHub 最新稳定 Release 是 `v2026.10.20.1`，包索引 Windows/Linux/macOS 的 latest 均指向 `2026.10.20.1`。本次升级基线采用它；后续修复发行后替换成真实发布的新版本号，不预设尚不存在的版本。

现有 `2026.08.02.2` 仅保留在旧环境迁移用例中，不能用于证明新版本首次使用正常。上一篇方案关于“d2x 暂不升级”的判断属于当时快照，本方案取代该版本建议。

## 2. 故障证据与未决项

| 项目 | 已确认事实 | 含义 |
|---|---|---|
| 故障提交 | `2a58b11` 把 d2x 从 `2026.08.02.2` 升至 `2026.10.20.1`；实际 CI checkout 为合并提交 `ef8b174` | 分析和复现应记录实际 checkout SHA |
| 失败环境 | Windows 2025、cold、遮蔽 MSVC；工具安装成功 | 不是安装步骤退出失败 |
| 首次启动 | `first-start.ndjson` 为 0 字节 | 尚未产生 session 或练习事件 |
| stderr | `provider idle-timeout: no output for 120s while running 'mcpp run -q -p d2x/buildtools -- describe' — killed` | 直接失败点为 describe 阶段静默超时 |
| checker 退出 | 记录 Provider 无响应，却退出 0 | CLI 没有传播基础设施失败 |
| README 对照 | 实际安装 `2026.08.02.2`；先运行 solutions，随后 checker 成功 | 同时有版本差异与预热差异，不能作为新版 cold 成功的证据 |

失败附件没有 mcpp 的底层构建日志，尚不能确定 120 秒内在下载、解压、构建 std 模块、编译 Provider，还是某一步阻塞。也不能仅凭这一轮结果认定新版引入了构建性能回归。

实施前先在相同 Windows 无 MSVC 环境分两份独立冷目录复现：一份直接 checker；另一份直接 `mcpp build -p d2x/buildtools` 并记录各阶段。保留进程、耗时、退出码、stdout/stderr、实际工具链。第二份用于定位，不能预热第一份。

README 当时运行旧版的具体原因同样待证：安装前索引未更新、索引快照/镜像滞后、全局选择与项目版本解析等均需记录验证。当前远端 latest 已正确，不能据此认定故障时客户端一定使用了同一索引。

## 3. d2x：区分启动与练习运行

### 3.1 有界启动期限

推荐新增 `provider_startup_timeout`，只控制初次 describe 命令从创建子进程到成功结束的总期限。建议默认 900 秒；用户可显式设置正值，新增配置不提供无限等待模式。保留 `provider_idle_timeout` 默认 120 秒，用于初始化成功后的 exercises/check 调用。

describe 阶段使用启动总期限，不再因首次构建 120 秒无输出而中断；总期限不随日志或 UI 提示刷新。启动期限到达即终止整个子进程树并报告启动超时。计时使用单调时钟，超时与取消均须等待清理完成。

这是通用 Provider 启动策略，不新增 build/run/test 协议动词，不让 d2x 识别 mcpp 包目录、编译器或课程构建细节。编译型课程、解释型课程共用现有 describe/exercises/check 协议。

配置读取、环境变量覆盖、交互配置保存和文档同步支持新字段。已有配置不填新字段即可获得新的启动默认值；已有 `provider_idle_timeout` 在后续调用中维持原语义。升级说明明确 describe 阶段改用单独期限。

### 3.2 启动期间可观察

启动命令前建立事件 sink，在 session 前允许发送宿主侧准备进度：阶段、已等待时长、总期限。print/TUI 显示“正在准备课程环境，首次运行可能需要数分钟”，每 10–20 秒更新等待信息。

这些提示只描述仍在等待，不能假装底层正在下载或编译，也不能作为子进程活性证明。UI 提示不重置期限。事件名称及字段在实现时写入协议文档并加兼容测试；session 的含义保持为练习清单和进度已就绪。CI 当前忽略非目标事件的机制需复核。

Provider stdout 保留协议解析；子进程 stderr 实时独立读取并呈现为诊断输出，避免 pipe 堵塞。未知 stdout 行保留在诊断日志中，不直接写入 checker NDJSON。分别保存 stdout/stderr 尾部，失败报告注明命令、工作目录、阶段、耗时、退出码/超时原因和日志位置。

若 mcpp 的 `-q` 抑制了底层输出，d2x 也无法恢复；课程构建入口是否取消 `-q` 需实测正常日志与协议解析兼容性，不能把捕获 stderr 当成一定可见构建进度。

### 3.3 错误应可被脚本判断

让 `checker::run` 返回状态，并由 `cmdprocessor` 传播到 CLI。配置缺失、实例锁冲突、启动失败/超时、Provider 协议错误、枚举失败应返回非零。`--emit-events` 同时输出结构化 error，包含稳定的错误类别及失败阶段。

区分合法空课程与 exercises 命令失败：transport 必须返回调用状态，不能以空 vector 丢弃失败原因。收到部分事件后子进程非零或超时也不能视为成功。

练习预期编译失败仍是 verdict，不变成 checker 基础设施失败；正常完成返回 0。Ctrl+C 沿用明确记录的正常交互取消约定，不能被误报为 Provider 故障。POSIX 和 Windows 均验证无遗留 shell、mcpp 或编译器进程。

### 3.4 d2x 主要改动位置

| 位置 | 责任 |
|---|---|
| `src/config.cppm` | 启动期限配置、校验、读写及覆盖规则 |
| `protocol/src/transport.cppm`、协议 process 模块 | 按调用阶段应用期限、stdout/stderr 并行读取、返回结构化调用状态、进程树终止 |
| `src/checker.cppm` | session 前准备进度、统一失败路径、返回状态 |
| `src/cmdprocessor.cppm` | CLI 退出状态传播 |
| print/TUI、协议文档与测试 | 新准备事件、失败反馈、老 Provider 兼容 |

不得只在课程里永久放宽所有调用的 idle timeout，也不能用宿主定时输出掩盖子进程挂死。已有 Provider check 内部心跳继续负责练习构建阶段；它无法覆盖 Provider 本体尚未生成的阶段。

## 4. d2mcpp：让安装版本与被测版本一致

### 4.1 公开入口使用最新稳定版

README 三种语言维持简洁流程，但显式在工具安装前更新索引：`xlings update` → `xlings install d2x mcpp -y`。实施时使用已安装旧版的场景验证该命令是否会选择最新版本；若实际需要更新/切换命令，以 xlings 当前支持的命令修正流程。

课程下载后必须安装课程 `.xlings.json` 声明的依赖，并在运行前确认实际版本。优先让 `d2x install` 在现有课程安装机制内完成该步骤；先核实 xlings 课程配方是否已经负责安装，避免重复下载或循环调用。不要仅在 CI 里增加隐藏准备步骤而让用户命令仍运行旧版本。

根 `.xlings.json` 升至当前最新稳定版 `2026.10.20.1`；联动修复版发布后再固定到新版本。准备脚本、Dev Container、升级脚本均使用同一声明，旧版只出现在迁移 fixture。

### 4.2 README 候选 CI 必须运行候选版本

保留真实 README 命令执行器和候选课程 Git URL 重定向。初始工具安装允许经过公开 latest 入口；下载候选课程后，执行用户同样会经历的项目依赖协调步骤，核验课程 pin 与实际可执行版本一致，再测试 checker。

记录两个阶段的版本：公开安装完成后、进入课程依赖安装完成后。打印实际路径及在课程工作目录执行的 `d2x --version`、`mcpp --version`，并在 checker 启动前断言。候选 pin 是新版本而实际仍是旧版时立即失败。

latest 检查与候选 pin 验收分开：定时/main job 对比稳定 Release、各平台已发布索引和公开安装后的版本；PR job 断言候选 pin。发布/索引尚未同步时报告具体不一致，不能悄悄回退旧版。候选预发布产物通过显式 CI 安装覆盖测试，不冒充公开 latest 已发布。

### 4.3 保持两种首次使用路径

- README 路径：安装 → 下载候选课程 → 依赖协调与版本断言 → solutions → checker 保存、推进、恢复。证明文档可执行。
- cold 路径：只安装声明依赖 → 版本断言 → 立即 checker。开始时断言无工具链缓存、Provider 产物和学习进度；禁止预先 build、describe、status、solutions。

`first-run.py` 保留完整事件断言，失败时把已保存 stderr 尾部直接打印到 Actions 日志；出现退出 0 但无 session 时仍失败。测试预算大于 d2x 启动期限并给首次 check 留出时间，避免外层测试先超时而看不到真实诊断。

## 5. 联动验收

| 范围 | 必须通过的结果 |
|---|---|
| d2x 启动超时单测 | 用可配置短期限模拟：静默超过 idle、但未超过 startup 后成功；真正超过 startup 必须失败。用虚拟/可注入时钟或短期限，避免每次测试等待数分钟 |
| d2x 失败注入 | describe 非零、空输出退出 0、无效协议、部分输出后失败、持续 stderr、大量输出、取消、超时均可定位且退出码正确 |
| d2x 运行阶段 | 初始化完成后仍按 idle 策略处理挂死；check 心跳可维持正常构建；准备提示不能让挂死进程无限存活 |
| d2x 兼容 | 既有配置与老 Provider 正常；print/TUI 均能看到准备和错误；事件 stdout 保持有效 NDJSON |
| d2mcpp 平台矩阵 | 最小 Linux 普通用户、macOS ARM64、Windows 2022/2025；Windows 2025 单列无 MSVC；每个平台分开 README/cold |
| 课程行为 | 首题预期失败 → 保存答案 → 通过 → 下一题 → 重启恢复；solutions 数量与仓库一致并全部通过 |
| 版本 | 候选 pin = 课程内实际运行版本；公开 latest = 发布与索引声明；旧版迁移后目录内实际使用新版 |
| 环境升级 | 保留练习和学习进度；重复 setup 不改变进度；Dev Container 初始化与重连均使用新版本 |
| 诊断 | 首次构建阶段与耗时可追踪；失败附件含版本、路径、checkout SHA、索引来源/revision（客户端可提供时）、工具链诊断、stdout/stderr 与事件流 |

首次响应时长分别记录启动准备和首题 check，不先承诺尚未测得的性能指标。900 秒为首轮有界预算建议，需根据无缓存矩阵实测调整；超过预算时保留明确失败，不能通过删除 cold 测试规避。

## 6. 交付次序

1. **取证与版本防漏**：补充 CI 版本/路径记录及断言，在独立环境复现 mcpp 构建静默阶段。对旧版迁移测试作明确标识。
2. **d2x 修复 PR**：启动期限、准备进度、诊断、退出状态与进程清理，一并加入可快速运行的失败注入测试。
3. **跨仓候选验收**：d2x CI 生成带 SHA/校验和的候选产物，d2mcpp workflow_dispatch 显式选择该产物，在完整首次使用矩阵验证。候选产物的来源、SHA 和实际版本写入结果，不要求先发布稳定版。
4. **发布与索引**：通过验收后发布 d2x 修复版，同步各平台包索引与分发资源；使用全新索引环境验证公开安装命令得到该版本。Release 存在不等于 latest 安装链已就绪。
5. **课程升级**：d2mcpp 固定修复版，合并 README/项目依赖协调与 CI 调整。重复跑 README、cold、升级及 Dev Container。三种 README、双语环境/入门说明在实施时同步。
6. **持续追新**：定时比较最新稳定版和课程 pin；生成升级结果/PR 的自动化另在实施时明确权限。此方案本身不发送通知、不创建 PR、不发布。

跨仓候选验证可通过人工触发完成首轮联调，先不引入双向自动触发。对未来最新稳定版的更新，沿用同一验收门槛；不能因某个平台失败而让 CI 静默换回旧版。

## 7. Review 决策

建议确认以下设计后进入实现：

1. 最新稳定版通过公开入口安装，课程固定最新且验收通过的具体版本，并用定时检查推动升级。
2. d2x 新增独立、有总期限的启动阶段；初始预算 900 秒，运行阶段保持 120 秒 idle；保留通用三动词 Provider 协议。
3. 准备信息、原始诊断、错误事件与非零退出码一起交付；不以仅加长 Python 等待时间替代。
4. README 候选版本与实际执行版本强制一致，cold 路径保持无预热。
5. 先跨仓验收候选产物，再发布、更新索引和课程 pin。

## 8. 资料与验证范围

- [失败作业](https://github.com/mcpp-community/d2mcpp/actions/runs/39407074670/job/118575491926) 与[失败附件](https://github.com/mcpp-community/d2mcpp/actions/runs/39407074670/artifacts/12257597444)：已下载并读取。
- [README 无 MSVC 对照作业](https://github.com/mcpp-community/d2mcpp/actions/runs/39407074670/job/118575492306)：已下载附件，确认实际安装旧版与 solutions 预热。
- [d2x 最新稳定 Release](https://github.com/d2learn/d2x/releases/tag/v2026.10.20.1)：已通过 GitHub API 核实 latest。
- [当前 d2x 包索引](https://github.com/openxlings/xim-pkgindex/blob/main/pkgs/d/d2x.lua)：已通过 GitHub API 核实各平台 latest。
- [d2x v2026.10.20.1 源码](https://github.com/d2learn/d2x/tree/v2026.10.20.1)：本地 checkout `77bc6d6`，已检查 config、checker、cmdprocessor、protocol transport 的相关实现。
- [此前 mcpp 升级方案](2026-10-19-mcpp-upgrade-and-first-run-plan.md)：保留历史记录，本方案更新 d2x 版本与冷启动策略。

本次未执行新的 Windows 复现、跨平台运行或候选产物验收；所有新增字段、事件和行为均为待实现设计，不代表当前 Release 已支持。
