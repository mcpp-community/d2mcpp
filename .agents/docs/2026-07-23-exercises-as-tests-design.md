# 练习即测试：d2mcpp 架构重设计（设计稿）

- 日期：2026-07-23
- 状态：**设计已讨论定稿，未实现**
- 前置阅读：
  - [`2026-07-20-mcpp-provider-reference.md`](2026-07-20-mcpp-provider-reference.md)（当前实现，本设计将取代其中的清单生成与判定部分）
  - d2x 仓库 `.agents/docs/2026-07-20-d2x-architecture-reference.md`（Provider 协议，本设计不改协议）
- 本文定位：第二轮重设计的**决策记录与实施蓝图**——双入口定位、练习即 tests/、无宏 harness、mcpp 上游改动清单

---

## 1. 目标与定位

### 双入口

| 入口 | 体验 | 谁负责 |
|---|---|---|
| `d2x checker` | **闯关游戏**：顺序推进、进度持久化、将来的随机抽题/错题重练/游戏化呈现 | d2x |
| `mcpp test` / `mcpp run` | **真实工程**：cd 进标准目录直接用 mcpp，进度表就是测试结果，零生成物零魔法 | d2mcpp + mcpp |

两条入口驱动**同一份练习、同一条判定链路**，不存在平行路径。

### 责任划分（重申并收紧）

- **d2x**：拥有学习循环和它的呈现。判定信号拿去做进度显示还是做成游戏，d2mcpp 不关心也感知不到。将来的「随机出题」等策略是 d2x 会话层的事——协议本就是 `check <id>` 按需判定，**协议与 d2mcpp 均无需改动**。若将来需要「同一题随机参数」，协议预留 `check <id> --seed N` 扩展位，现在不做。
- **d2mcpp**：提供关卡（练习内容）+ 判定事实（编译过没过、断言挂没挂、路障拆没拆）。与 mcpp 和模块化深度绑定：harness 是纯 C++ 模块，判定复用 `mcpp test`。
- **mcpp**：通用构建/测试工具。本设计需要它补三个**通用**能力（§4），均为 cargo/ctest 已验证的形态，不含任何 d2x 定制逻辑。

### 教学标准约定（维持既有决策）

所有练习统一按 **c++23** 编译；`cppNN/` 表示**特性引入于哪个标准**，内容只用该标准的特性，不改变编译参数。教学点必须有断言守着（防静默漂移，见 provider-reference §8）。

---

## 2. 方案对比与结论

| 方案 | 形态 | 结论 |
|---|---|---|
| **A. 标准即工程，练习即 tests/** | `cpp11/` 是真实 mcpp 工程，练习是 `tests/<章节>/<序号>.cpp` | **采纳** |
| B. 章节即工程 | 每章一个 mcpp 工程，workspace 挂 30+ member | 否。频繁 cd + 30 多个 mcpp.toml 纯属维护负担；「章节」用 tests/ 子目录即可表达；仅当章节需要各自不同依赖时才值这个价（现状 52 题里为零） |
| C. 不改上游（现状延续） | Provider 继续生成 `_current` 清单 | 否。原生体验只能到「mcpp run 单题」，拿不到「mcpp test = 进度表」这个最贴合闯关直觉的形态 |

方案 A 的前提是 mcpp 上游三改动（§4）。实验已证实当前行为不可用：**一个编译不过的测试 → 整体 `error: build failed`，零测试执行**（实测 mcpp v0.0.99，三个测试一好一运行错一编译错）。而「填空未填 = 编译不过」是练习的常态。

---

## 3. 目录布局

```
d2mcpp/
  mcpp.toml                  workspace members = [harness, cpp11, cpp14, ...]
  harness/                   独立 mcpp 包（standard c++23，纯模块，无宏）
  cpp11/
    mcpp.toml                真实工程：standard = "c++23"，[dependencies] harness (path)
    tests/
      00-auto-and-decltype/0.cpp
      00-auto-and-decltype/1.cpp
      01-nullptr/0.cpp
      ...
  cpp14/ ...同构
  solutions/
    cpp11/00-auto-and-decltype/0.cpp   结构镜像 tests/（zh/en 共用一份，维持现状）
```

- 章节 = tests/ 下的子目录（`mcpp test` 本就递归收集 `tests/**/*.cpp`）。
- id 推导沿用目录约定：`cpp11/tests/00-auto-and-decltype/0.cpp` → `cpp11-00-auto-and-decltype-0`，order 公式不变（std_rank × 100000 + 章节 × 100 + 序号）。id 白名单 `[A-Za-z0-9._-]` 在源头拒绝，理由不变（注入纵深防御）。
- `dslings/` 目录退役；`hello-mcpp` 入门练习移入独立的 `intro/` 工程（同构，一个测试），并补参考答案（闭掉「e2e 只能 SKIP 它」的缺口）。
- **`.d2x/build/` 不复存在**：无生成清单、无 `_current` 改写。`.d2x/state.json`（d2x 写）保留。
- clangd：由 mcpp 为 tests/ 生成的 `compile_commands.json` 覆盖（实施时验证：练习编译不过时条目是否仍生成——若否，此项是上游第四个小改动）。

### en 练习

维持「zh/en 互斥启用」语义。en 练习放 `cpp11/tests-en/` 还是独立 `cpp11-en/` 工程，实施时按 mcpp 对非常规测试目录的支持度定（倾向前者 + mcpp.toml 可配 tests 路径；这也是通用能力）。**本条是实施期待定项，不影响架构。**

---

## 4. mcpp 上游改动清单

三个改动，全部是通用能力，d2mcpp 只是第一个消费者。

### 4.1 逐测试编译隔离

`tests/` 下每个 `.cpp` 本来就是独立二进制 target。改动后：单个测试编译失败 → **该测试**标记 fail（reason = compile），其余照常编译运行；整体退出码非 0 但不再是「build failed 零执行」。ctest/meson 的既有行为；「写坏一个测试导致其他测试全跑不了」对任何 mcpp 用户都是缺陷。

**验收**：一好一运行错一编译错的三测试工程，`mcpp test` 三个都有结果，好的那个真的执行了。

### 4.2 测试过滤

`mcpp test <pattern>`，按测试名（tests/ 下相对路径去扩展名）子串或 glob 过滤。对齐 `cargo test <name>`。

**验收**：`mcpp test 00-auto` 只编译运行匹配的测试；Provider 的 `check <id>` 走 `mcpp test --filter <精确名>`。

### 4.3 机器可读结果 `--message-format json`

每测试一行 NDJSON：

```jsonc
{"test":"00-auto-and-decltype/0",
 "status":"pass|compile_fail|run_fail",
 "exit_code":0, "signal":null,
 "compile_output":"...编译器原文...",
 "run_output":"...合并的 stdout+stderr..."}
```

**必须区分错误层级**：包级构建失败（harness 或 lib target 挂了）≠ 测试级编译失败。包级失败单独一条 `{"error":"package", ...}`，否则 52 题全标红会把「课程基础设施坏了」误报成「你全做错了」（对应 d2x 侧「describe 失败 = Provider 挂了，而非没有练习」原则）。

对齐 `cargo --message-format=json`；任何 CI/IDE 集成都需要。

### 顺带修复

嵌套 mcpp 的 `LD_LIBRARY_PATH` 段错误在上游正式修掉（mcpp spawn 子进程时不让私有 glibc 路径泄漏给非 mcpp 二进制），随后删除 d2mcpp `runner.cppm` 与 d2x `platform.cppm` 两处 `unsetenv` workaround。

### 待验证的第四项（小）

练习编译不过时 `compile_commands.json` 是否仍含该条目（clangd 依赖）；若否，补之。

---

## 5. 判定链路

```
harness(运行期事实) → mcpp test(编译/退出码事实) → Provider(合并成 verdict) → d2x(呈现，d2mcpp 不关心)
```

### 三个核心要素的唯一事实来源

| 要素 | 来源 | 说明 |
|---|---|---|
| 当前判定的练习文件 | **Provider 的目录约定**（不经过 mcpp） | 发现阶段建立 id ↔ 绝对路径映射；mcpp JSON 里的测试名仅用于对账 |
| 编译期错误 | mcpp JSON（`status:compile_fail` + `compile_output` 原文） | 原文转协议 `output` 事件；`unknown type name 'D2X_YOUR_ANSWER'` 指着要填的行，是教学主通道 |
| 运行期错误 | mcpp JSON（退出码/信号）× 侧信道（断言详情/wait） | 侧信道逐条追加写，崩溃前的记录保得住 |

### 判定顺序（Provider 内，语义与旧 `judge_run` 一致）

| 观察 | Verdict |
|---|---|
| mcpp 报 `compile_fail` | `fail` + 编译输出 |
| 侧信道有 `ok:false` | `fail` + 每条失败转一个 Diagnostic |
| 无断言失败但退出码非 0（纯崩溃） | `fail` + 运行输出 |
| 无失败、有 `wait` | `blocked` |
| 侧信道文件不存在（未用 harness 的纯观察题） | 退回「退出码 0 = pass」 |

结构化的**编译期** diagnostics（file/line 进协议字段）仍是既有缺口，本设计不倒退也不顺带解决；将来经 `-fdiagnostics-format=json` 做独立增量。

---

## 6. 无宏 harness

### 关键事实：填空占位符不需要是宏

实验对比（clang 21，c++23）：

| 方案 | 报错 |
|---|---|
| 宏展开为空（现状） | `use of undeclared identifier 'b1'` ——指着错误位置、级联报两次 |
| **裸标识符（新）** | `unknown type name 'D2X_YOUR_ANSWER'` ——正好指着要填的地方，只报一次 |

所以 `D2X_YOUR_ANSWER` 保留拼写但**降格为纯约定**：不定义在任何地方、不需要任何头文件、在模块化练习里天然可用、拷进 Compiler Explorer 也成立。旧文档「宏跨不了模块边界，模块化章节填空无约定」这个缺口是**伪问题，就此关闭**。

### harness 包接口（`import d2x.harness;`）

```cpp
export namespace d2x {
    // 断言：source_location 自动带 file/line；同时打印人读输出（✅/❌ 逐条对照，教学的一部分）
    // 并在 D2X_RESULT_FILE 设置时逐条追加写侧信道
    bool check(bool ok, std::string_view what = {},
               std::source_location loc = std::source_location::current());
    template<class A, class B>
    bool check_eq(const A& a, const B& b, std::string_view what = {},
                  std::source_location loc = std::source_location::current());

    // 路障：打印「读完后删掉这一行」提示并记入侧信道
    void wait(std::source_location loc = std::source_location::current());
}
```

- **退出码机制**：首次调用任一 API 时注册 `atexit` 处理器；进程正常退出时若有失败断言或未拆的 wait → `_Exit(1)`。这让裸 `mcpp test` 不需要懂任何 d2x 概念就能显示对错——练习的 `main` 不需要 `return d2x::result()` 这类样板。
- **`#expr` 字符串化的损失与补偿**：函数版拿不到表达式原文（c++23 无反射）。补偿：`what` 可选参数供作者写语义标签（`check_eq(a1, a2, "auto 与 decltype 推导应一致")`——教学价值高于表达式原文）；不传时人读输出退化为值对照 + file:line，学习者看得到出错行。将来 c++26 反射可无侵入恢复原文。
- 旧的 `d2x/cpp/common.hpp` 经典 include 路径、`honly_logger.hpp`、`D2X_YOUR_ANSWER`/`d2x_assert*`/`D2X_WAIT` 宏全部退役。纯观察型练习依旧可以零依赖（不 import 任何东西）。
- harness 作为独立 mcpp 包按公共契约维护（本次重设计即冻结接口的时机），将来发 registry 供其他 C++ 课程复用。

### 侧信道 schema v2

harness 与 Provider 之间的契约，本次显性化 + 加版本：

```jsonc
{"v":2,"kind":"assert","ok":false,"what":"...","expected":"1","actual":"2",
 "file":"cpp11/tests/00-auto-and-decltype/0.cpp","line":38}
{"v":2,"kind":"wait","file":"...","line":45}
```

- `D2X_RESULT_FILE` 未设置时只打印不写文件（学习者直接跑二进制零摩擦），语义不变。
- 逐条追加而非退出时统一写（段错误保留崩溃前记录），语义不变。
- 写端负责完整 JSON 转义（`"` `\` 控制字符）；读端（Provider）字段固定、无嵌套，仍可免 JSON 库。
- `file` 按编译器给的路径记录，**Provider 在协议边界统一还原为绝对路径**（d2x 靠它开编辑器/监听）。`-fmacro-prefix-map` 不再需要——source_location 在真实工程目录下编译，路径本来就正常。

---

## 7. Provider 瘦身

| 现状 | 新形态 |
|---|---|
| 生成 `.d2x/build/` 三套清单 + `_current` 改写舞蹈 | **全部删除** |
| 自己 popen `mcpp build/run` 并解析 wait status | `mcpp test --filter <id> --message-format json` 一次调用 |
| 判定 = 自己读侧信道 + 自己算退出码 | 判定 = 合并 mcpp JSON × 侧信道（§5 顺序表） |
| 发现 = 扫 `dslings/` | 发现 = 扫 `cpp*/tests/`（约定同构，公式不变） |

保留：id 白名单、`describe/exercises/check` 三动词、协议事件格式（对 d2x **完全透明**，d2x 零改动）。

### 每练习编译选项

文件头 `// d2x:cxxflags:` 指令在 tests/ 模式下无法由 Provider 注入（编译由 mcpp 统一驱动）。两个候选：mcpp.toml 支持 per-test 覆盖（通用能力，meson 有先例），或 mcpp 认文件头指令（特化，倾向否）。现存仅 2 题需要（`04-rvalue-references`、`07-constexpr-0`）。**实施期决策项**；若 mcpp.toml 方案成立则指令退役。

---

## 8. e2e 验证新形态

不变的两条硬断言：**每题未完成时不通过、每份参考答案放进去后通过**。

- 实现简化：dirty-tree 防护 → 逐题「拷 solution 覆盖 → `mcpp test --filter <id>` → 还原」，或整体「全部覆盖 → `mcpp test`（此时应全绿）→ 还原」。后者一次构建验全部，优先。
- 防空转（`pass == 0` 直接失败）与脏树检查两道防线**原样保留**（各自都咬过人，见 provider-reference §7）。
- `intro/hello-mcpp` 补参考答案后进入 e2e，不再 SKIP。

---

## 9. 迁移影响面

| 项 | 工作量性质 |
|---|---|
| 52 题从 `dslings/` 迁到 `cpp*/tests/` + 宏改函数（`d2x_assert_eq(a,b)` → `d2x::check_eq(a,b)`、`D2X_WAIT` → `d2x::wait();`、`#include <d2x/cpp/common.hpp>` → `import d2x.harness;`） | 机械，可脚本化 + 人工过一遍 what 标签 |
| 书本（book/）里引用练习路径/代码片段 | 跟随更新，路径变更是主要部分 |
| solutions/ 同步迁移 | 机械 |
| `.agents/skills/d2mcpp-authoring/` 撰稿流程 | 重写「新增练习」章节：放文件进 `cpp*/tests/<章节>/` 即完成注册（约定不变），验证命令改为 `mcpp test <id>` |
| CI workflow | e2e 命令更新；mcpp 版本 pin 到含三改动的版本 |
| d2x 仓库 | **零改动**（协议不动）；仅 smoke CI 的 d2mcpp 分支引用需协调 |

### 实施顺序（跨仓库依赖）

```
mcpp 上游三改动（4.1→4.3，可并行）+ LD_LIBRARY_PATH 修复 → 发版
  → d2mcpp：harness 重写 → 目录迁移 → Provider 瘦身 → e2e/CI → 文档与撰稿技能
  → 与 d2x #31 / d2mcpp #84 的合并顺序问题一并协调（互相咬合，需同步合并 + d2x 发新版）
```

本设计建立在 #31/#84 两个 draft PR 之上（协议、三态、侧信道思想全部沿用），是它们的演进而非推翻。

---

## 10. 本设计关闭/遗留的缺口

| provider-reference §10 的缺口 | 状态 |
|---|---|
| 模块化练习填空占位符无约定 | **关闭**（伪问题：裸标识符即编译错误，且报错更好） |
| `hello-mcpp` 无参考答案 | **关闭**（迁入 intro/ 时补） |
| 新 CI 从未真跑过 | 已在 PR #84 上跑绿（2026-07-19），本设计后需再验 |
| macOS / Windows 从未验证 | 遗留；Provider 瘦身后 `_popen`/`_putenv_s` 面积大幅缩小，风险随之降低 |
| 编译错误未结构化 | 遗留；独立增量（`-fdiagnostics-format=json`） |
| Provider 无超时 | 遗留（d2x 侧缺口） |
| en 练习的目录形态 | 新增待定项（§3） |
| per-test 编译选项的 mcpp 承载方式 | 新增待定项（§7） |
