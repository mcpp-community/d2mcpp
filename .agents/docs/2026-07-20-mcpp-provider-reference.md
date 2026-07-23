# d2mcpp Provider 与练习脚手架 参考

- 日期：2026-07-20
- 分支：`feat/mcpp-provider`
- 配套调研：[`2026-07-19-mcpp-replace-xmake-research.md`](2026-07-19-mcpp-replace-xmake-research.md)（xmake → mcpp 的可行性论证与 rustlings/cargo 横向对照）
- d2x 侧协议规范：d2x 仓库 `.agents/docs/2026-07-20-d2x-architecture-reference.md`
- 本文定位：**当前实现的参考手册**——目录约定、生成物、判定机制、维护须知


> **⚠️ 2026-07-23 起本文的 §4(清单生成)与 §5(判定机制)已被「练习即测试」迁移取代,现状见 [`2026-07-23-exercises-as-tests-reference.md`](2026-07-23-exercises-as-tests-reference.md)。**
---

## 1. 全景

xmake 已完全退役。构建链路是：

```
d2x（通用框架，零内置后端）
 └─ Provider Protocol (NDJSON) ─→ d2x/buildtools/mcpp/   ← 本仓库自己的 Provider（C++26）
                                    ├─ 目录约定发现练习
                                    ├─ 生成 mcpp workspace 清单
                                    ├─ 调 mcpp build/run
                                    └─ 读侧信道判定通过
                                          ↓
                                        mcpp（未做任何改动）
```

### 提交进仓库的包

```
mcpp.toml                      [workspace] members = ["d2x/buildtools/mcpp", "dslings/harness"]
d2x/buildtools/mcpp/           Provider，standard = "c++26"
dslings/harness/               练习脚手架库，standard = "c++23"
```

### 生成物（gitignore）

```
.d2x/build/mcpp.toml           独立 workspace 根
.d2x/build/<std>/mcpp.toml     该标准下全部 target —— 供 clangd
.d2x/build/_current/mcpp.toml  只含当前一题 —— 供 checker
.d2x/build/_current/result.ndjson   判定用侧信道
.d2x/state.json                学习者进度（d2x 写）
```

---

## 2. 引导：零脚本、Windows 安全

`.d2x.json`：

```json
{ "buildtools": "mcpp run -q -p d2x/buildtools/mcpp --" }
```

d2x 拼接后得到 `mcpp run -q -p d2x/buildtools/mcpp -- check <id>`。**从仓库根执行、无需 cd、首次自动构建 Provider**，暖开销约 26ms。

d2x 从不 `chdir`（它在静态初始化时捕获 CWD），所以免 `cd` 是硬要求。注意 `mcpp` 的 `-p` 匹配的是**目录 basename 或完整相对路径**，不是包名。

### ⚠️ 嵌套 mcpp 与 `LD_LIBRARY_PATH`

`mcpp run` 会把 `LD_LIBRARY_PATH` 指向它私有的 glibc 并注入子进程。Provider 再去 spawn 嵌套的 `mcpp` 时被迫加载错配的 glibc，**在动态链接器里段错误**（冷启动稳定复现，输出只剩 `<pid>:\t__vdso_time` 这样的 trace 残片）。

`runner.cppm` 在每次 spawn 前 `unsetenv("LD_LIBRARY_PATH")` 绕过；mcpp 会为它自己的子进程重新设置正确的值。

d2x 侧对同一问题早有相同处理（`platform.cppm`），仓库里还留着 `workaround_ld_library_path_issue` 分支——**这是 mcpp 的既有问题，值得单独向上游报**。

---

## 3. 练习发现：目录约定 + 就近指令

**没有声明文件。** 放一个 `.cpp` 进去就是全部工作。

rustlings 最贵的一课是 PR #1355：Rust edition 同时写在 `rustc` 参数和 `rust-project.json` 里，两边漂移酿成 bug。**任何独立声明文件都是第二套真相源。** 这里的真相只有两处，且都无法漂移：目录结构，和练习文件自己的头部注释。

### 目录布局

```
dslings/hello-mcpp.cpp            入门练习（chapter = intro）
dslings/<std>/NN-topic-K.cpp      zh 练习
dslings/en/<std>/NN-topic-K.cpp   en 练习（lang=en 时启用，与 zh 互斥）
solutions/<std>/NN-topic-K.cpp    参考答案（zh/en 共用同一份）
```

### id / order / chapter 的推导

| 文件 | id | chapter | order |
|---|---|---|---|
| `dslings/cpp11/00-auto-and-decltype-0.cpp` | `cpp11-00-auto-and-decltype-0` | `cpp11/00-auto-and-decltype` | 1100000 |
| `dslings/cpp11/03-trailing-return-type.cpp` | `cpp11-03-trailing-return-type` | `cpp11/03-trailing-return-type` | 1100300 |
| `dslings/hello-mcpp.cpp` | `hello-mcpp` | `intro` | -100000 |

`order = std_rank × 100000 + 章节号 × 100 + 练习序号`。尾部 `-<数字>` 是序号，没有就当 0（仓库里 03/04 两章确实缺 `-0` 后缀，规则把它们归一化）。

### 每练习编译选项

写在练习文件自己的头部注释里（只扫前 40 行）：

```cpp
// d2x:cxxflags: -O0 -fno-elide-constructors
```

仓库现存用例：`04-rvalue-references`（观察移动）、`07-constexpr-0`（`-Wpedantic -Werror` 让 VLA 成为错误）。

### id 白名单

id 只允许 `[A-Za-z0-9._-]`，违反者**被拒绝并打印到 stderr**，不进入枚举。

因为 id 有两个危险去向：d2x 拼进 shell 命令、我们写进生成的 TOML（`[targets.<id>]`）。在源头挡住比在两个下游各自转义更可靠——这类文件名本就是笔误或恶意 PR，与其想办法安全传递，不如让作者改名。

---

## 4. 双 member 布局

**硬约束：dslings 的练习默认就编译不过**（49 个带 `D2X_YOUR_ANSWER`），而 mcpp 的 `build` 和 `run` 都会先全量构建整个包——一个坏兄弟拖垮全部且零产物。实测：

```
member 含全部 3 题（后两题未做完）→ mcpp build -p → exit=1   ← 当前这题被拖挂
只生成当前这一题                    → 0.071s，exit=0
```

**member 粒度（标准/特性/练习）本身不解决逐题隔离。** 所以分两个：

| 目录 | 内容 | 用途 |
|---|---|---|
| `.d2x/build/<std>/` | 该标准下全部 target | clangd —— 即使多数练习编译不过，mcpp 仍生成完整 `compile_commands.json`，IDE 对每题都有正确编译参数 |
| `.d2x/build/_current/` | 只含要验证的那一题 | checker |

实测动态改写清单代价极低：切题 0.118s、切回 0.018s，**fingerprint 目录始终只有 1 个**（改写 target 集合不会让缓存爆炸）。

### 其他生成细节

- `main` 用 `../../../` 逃逸出包根，**练习源文件原地不动**
- 脚手架走 `[dependencies] path` 依赖，而非把仓库根塞进 include 搜索路径（后者会让练习能 `#include` 仓库里任何文件）
- `-fmacro-prefix-map=<repo_root>/=` 让 `__FILE__` 相对仓库根，学习者看到的报错不再顶着一长串 `/home/...`
- 所有清单**内容比对后才落盘**，不推进 mtime，保住 mcpp 的快速路径
- `check` 会先 `write_full` 再 `write_current`——全新仓库上学习者直接跑 `d2x checker` 时根清单还不存在，否则 mcpp 报退出码 2

---

## 5. 判定机制

### 侧信道，不是扫 stdout

判定原先 100% 是带内信号：断言失败**并不改变退出码**（实测 exit=0），所以只能扫 stdout 找 `❌`。这会误判——一个断言全过的正确解答，只要在说明文字里打了个 `❌` 就被判失败（已复现）。反过来，输出截断或 Windows 控制台代码页不对，判定同样失效。

现在 stdout 回归「给人看」，判定走侧信道：

```jsonc
{"kind":"assert","ok":false,"expr":"a == b","expected":"1","actual":"2",
 "file":"dslings/cpp11/xx.cpp","line":33}
{"kind":"wait","file":"dslings/cpp11/xx.cpp","line":40}
```

**逐条追加而非退出时统一写** —— 练习段错误时，崩溃之前的断言结果照样保留。

`D2X_RESULT_FILE` 环境变量未设置时 harness 只打印不写文件：**学习者直接跑二进制零摩擦**。

### 判定顺序

| 侧信道内容 | Verdict |
|---|---|
| 有 `ok:false` | `fail` + 每条失败转一个 `Diagnostic` |
| 无失败但退出码非 0 | `fail` |
| 无失败、有 `wait` | `blocked` |
| **文件不存在**（练习没用 harness） | 退回「编译通过 + 退出 0 = `pass`」 |

最后一条让 **harness 自动变成可选的**，不需要额外机制：纯观察型练习（现有 18 个只用 `D2X_WAIT`）可以写成零依赖的纯 C++ 文件，学习者能原样拷进 Compiler Explorer。

### 路径：展示 vs 定位

侧信道里的 `file` 因 `-fmacro-prefix-map` 是相对路径（学习者看着舒服），但协议要求绝对路径（d2x 靠它开编辑器、监听变更）。**在协议边界上还原成绝对。**

---

## 6. 练习脚手架 `dslings/harness/`

按库设计，目前住在本仓库，将来可整体抽成独立 mcpp 包给其他 C++ 课程复用。

### 两条路径不等价

| 设施 | `#include <d2x/cpp/common.hpp>` | `import d2x.harness;` |
|---|---|---|
| 断言 | `d2x_assert` / `d2x_assert_eq` 宏 | `d2x::check` / `check_eq`，`source_location` 自动带 file/line |
| 路障 | `D2X_WAIT` | `d2x::wait()` |
| 填空占位 | `D2X_YOUR_ANSWER` | **无等价物** |

**宏无法跨模块导出**，这是 C++ 的硬约束而非疏漏。`D2X_YOUR_ANSWER` 必须展开为空才能制造编译错误，本质就是宏。**cpp20/cpp23 等模块化章节需要另设填空约定——这是个尚未决策的缺口。**

`include/` 下保持 `d2x/cpp/common.hpp` 这个既有路径，所以 104 个练习和书本一行都不用改。路径改名可以以后单独做。

### 维护须知

`d2x_assert_eq` 的**所有分支**都必须走 `d2x::detail::show()`，不能用裸 `std::to_string`——后者没有 `std::string` / `const char*` / scoped enum 的重载，一旦有练习比较这些类型就会硬编译失败。（曾经上报分支用了 `show()` 而日志分支没用，是个潜伏的地雷。）

---

## 7. 测试

### `d2x/buildtools/mcpp/tests/e2e.sh`

断言两件事，缺一不可：**每个练习未完成时不通过**、**每个参考答案放进去后通过**。等价于 rustlings 的 `cargo dev check --require-solutions`。

走的是 `d2x checker` 内部同一条 Provider 路径，所以绿灯等价于学习者本地能跑通，而非另一条平行路径。

**两道防线：**

1. **防空转**——`pass == 0` 时直接判失败。旧 CI 就是这么绿了很久的：它只挑 `-ref` 目标，而 `solutions/` 早被注释掉，循环一次都没进，job 照样退出 0；第 60 行的 `wc -l` 对空串还打印 "Found 1 reference targets" 把空转掩盖了。同类问题在本脚本自己身上也犯过一次（`${sol#en/}` 剥离顺序错，导致所有英文答案静默 SKIP）。

2. **脏树检查**——练习目录有未提交改动时**拒绝运行**并列出文件。脚本会把参考答案覆盖到练习上再还原，天然会吃掉未提交的改动，这个陷阱咬过两次。

### 当前状态

| 项 | 结果 |
|---|---|
| Provider 枚举 | 52 题（zh：1 + 49 + 2；en 同样 52） |
| 端到端 zh | 51/51 参考答案通过 |
| 端到端 en | 51/51 参考答案通过 |
| d2x session 单测 | 28/28 |
| 事件流 | 纯 JSON，0 污染 |
| 注入防护 | 恶意文件名被拒绝，命令未执行 |

---

## 8. C++ 标准与教学漂移

**所有练习按 `c++23` 编译。** mcpp 目前硬拒 `c++11/14/17/20`（`src/manifest/types.cppm` 的白名单），本方案选择不改 mcpp 上游。因此 `cppNN/` 目录现在表示**特性引入于哪个标准**，不再改变编译参数。

### 已知代价

`04-rvalue-references` 的移动构造教学点。C++17 起 prvalue 直接初始化目标（保证复制省略），`-fno-elide-constructors` 再也无法让 `Object obj = Object();` 产生移动构造。

**修法：** 改成从具名对象 `std::move`（任何标准下都必然调用移动构造），并补一条 `d2x_assert(move_ctor_calls >= 1)`。

**更重要的是那条断言。** 漂移之所以能静默发生，正是因为从前没有任何断言检查它——输出少一行没人发现。现在它是 CI 可检测的事实。

> **教训：教学点如果没有断言覆盖，它的消失就是不可见的。** 新增练习时应问：这一课的核心观测点，有断言在守着吗？

量化过的风险面：51 个参考答案在「原标准 vs c++23」下逐一编译并 diff 运行输出，**50 个完全一致，仅此 1 个漂移**。但随着课程往 cpp17/20 扩写，这类静默漂移只会变多。

---

## 9. 撰稿流程的变化

`.agents/skills/d2mcpp-authoring/` 已同步更新：

- **练习不再需要注册**——放进 `dslings/<std>/` 即被目录约定发现
- 新增 `<std>` 章节只需建目录 + 改两个 `SUMMARY.md`，无构建文件接线
- 每练习编译选项改为文件头部 `// d2x:cxxflags:` 指令
- 验证命令改为 `mcpp run -q -p d2x/buildtools/mcpp -- check <id>` 或整体 `bash d2x/buildtools/mcpp/tests/e2e.sh`

---

## 10. 已知缺口

| 缺口 | 影响 |
|---|---|
| **模块化练习的填空占位符无约定** | `D2X_YOUR_ANSWER` 是宏，跨不了模块边界；cpp20/cpp23 章节需先决策 |
| **macOS / Windows 从未验证** | `_popen`、`_putenv_s` 的 `#ifdef` 分支全是纸面推断 |
| **新 CI 从未真跑过** | workflow 是手写的，`xlings install -y` 在 CI 环境能否装上 mcpp 未验证 |
| **`diagnostics` 只覆盖运行期断言** | 编译错误尚未解析成结构化诊断 |
| **Provider 侧无单测** | discovery 的 id/order/chapter 推导、manifest 生成都只被端到端间接覆盖 |
| **`hello-mcpp` 无参考答案** | e2e 只能 SKIP 它，入门练习实际未被验证 |
| **`solutions/` 无 en 镜像** | 设计如此（共用一份），但意味着 en 专有的措辞错误无法被答案校验发现 |
