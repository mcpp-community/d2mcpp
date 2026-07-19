# 调研报告：用 mcpp 替代 xmake 支撑 d2mcpp 全项目

- 日期：2026-07-19
- 范围：d2mcpp（课程仓库）、d2x（课程驱动 CLI）、mcpp 0.0.99（构建工具）
- 结论一句话：**当前不可整体替换；但经 §4 的 rustlings/cargo 对照修订后，真正的阻塞只剩 1 个——mcpp 的 `[package].standard` 不接受 c++11/14/17/20。推荐"按标准分 workspace member（5~10 个）+ 每 member 内多 target + ninja 逐 target 隔离"。**
- ⚠️ 阅读顺序：§1-§3 是初版结论（单包 155 target、3 个上游需求）；**§4 基于 rustlings/cargo 的横向对照做了架构修订，以 §4.4 为准。**

---

## 1. 现状：xmake 在 d2mcpp 里承担什么

### 1.1 集成缝在哪

d2mcpp 不直接暴露 xmake 给学习者。三个 README 里 `xmake` 出现 **0 次**，学习者的唯一入口是 `d2x checker`。xmake 通过一层薄适配器接入：

```
.d2x.json:  "buildtools": "xmake d2x-buildtools"
                 ↓
xmake.lua:1  add_plugindirs("d2x/buildtools")
                 ↓
d2x/buildtools/xmake/{xmake.lua, main.lua}   ← 实现 init / list / build / run
```

契约写在 `d2x/buildtools/README.md`，只有 4 个命令。d2x 侧的实现（`d2x/src/buildtools.cppm:28-42`）是裸字符串拼接 + `popen`：

```cpp
auto init()  const { return platform::run_command_capture(bin + " init"); }
auto list()  const { return platform::run_command_capture(bin + " list"); }
auto build(const std::string& t) const { return platform::run_command_capture(bin + " build " + t); }
auto run  (const std::string& t) const { return platform::run_command_capture(bin + " run " + t); }
```

`bin` 完全来自配置，**不限定是 xmake**——写成 `./d2x/buildtools/mcpp/adapter` 也能跑。这是好消息：适配器不需要 mcpp 提供插件机制。

### 1.2 契约的精确要求（迁移必须逐条满足）

| 项 | 要求 | 出处 |
|---|---|---|
| `list` 输出 | 每行 `<name>@ <绝对路径>, <绝对路径>...`，`@` 必须**恰好出现一次**，否则该行被**静默丢弃** | `d2x/src/buildtools.cppm:76-100` |
| 路径 | 事实上必须绝对（`utils::read_file_to_string` 会抛异常，编辑器打开、mtime 轮询都按 CWD 解析） | `d2x/src/utils.cppm:66` 的 TODO |
| 排序 | 存进 `std::map`，即**按 target 名字典序**决定练习顺序 | `buildtools.cppm:21` |
| 退出码 | `build`/`run` 必须 `exit == 0` 才算通过；不解析编译器错误文本 | `d2x/src/checker.cppm:18-26` |
| 通过判定 | build==0 **且** run==0 **且** 输出无 `❌` **且** 无 `D2X_WAIT` | `checker.cppm:80-127` |
| 环境 | d2x 调用前会**清空 `LD_LIBRARY_PATH`**，且从不 `chdir` | `d2x/src/platform.cppm:51-54` |
| 噪声容忍 | 不匹配的行被忽略；ANSI 转义会被 trim 掉 | `d2x/src/utils.cppm:32-56` |

`init` 会在 `load_buildtools()` 里被**无条件调用一次且忽略返回值**（`buildtools.cppm:109`），mcpp 适配器可以让它做真正的初始化工作而不怕失败传播。

### 1.3 target 全景（迁移工作量的真实分母）

11 个 `xmake.lua`，153 处 target 声明。实际参与构建的只有两套之一（zh / en 互斥，同名）：

| lang | target 数 | 组成 |
|---|---|---|
| `en`（`option` 默认，且是提交进仓库的 `.d2x.json` 值） | 51 | 1 hello + 48 en/cpp11 + 2 en/cpp14 |
| `zh` | 52 | 1 hello + 49 cpp11 + 2 cpp14 |

- **`dslings/cpp17`、`cpp20`、`cpp23` 全是 6 字节的 README 占位符**，0 个 `.cpp`，没有任何 `includes()` 引用。课程实际只写到 cpp11 + 2 个 cpp14 练习。
- **`solutions/` 51 个 `-ref` target 完全没被构建**——`xmake.lua:6` 的 `includes("solutions/xmake.lua")` 被注释掉了（commit `307dc8a`）。
- **每个 target 都是"一个 `.cpp`、无依赖"**：`add_files` 带多文件/通配符 = 0 处，`add_deps`/`add_packages`/`add_defines`/`add_rules`/自定义 hook = 0 处。这是迁移最有利的事实。

需要特殊处理的只有 **32 个 target**（占 152 的 21%），且模式极少：

| 特性 | 出现次数 | 涉及 target |
|---|---|---|
| `set_kind("binary")`（显式，语义上是默认值，可直接丢弃） | 23 | 各 dir 前几个 |
| `add_cxxflags("-fno-elide-constructors")` + `set_optimize("none")` | 3 | `04-rvalue-references` ×{zh,en,sol} |
| `add_cxxflags("-Wpedantic -Werror")` | 3 | `07-constexpr-0` ×{zh,en,sol} |
| 单 target `set_languages("c++17")` | 3 | `08-literal-type-0` ×{zh,en,sol} |
| 单 target `set_languages("cxx11")` | 2 | `00-0-hello-mcpp` ×{zh,en} |
| 文件级 `set_languages` | 7 个文件 | — |
| Windows 分支（`set_encodings` / `set_toolchains("gcc")` / `add_ldflags("-static")`） | 1 处 | `dslings/xmake.lua:1-5` |
| `add_includedirs(".")` | 1 处（根） | **155/155 个 `.cpp` 都靠它** `#include <d2x/cpp/common.hpp>` |

---

## 2. mcpp 能力核对（含实测）

以下带 ✅/❌ 的结论**均为本机 mcpp 0.0.99 实测**，非仅读文档。

### 2.1 能做到的

**✅ 单包多可执行 target。** `[targets.<name>] kind="bin" main="..."`，每个 target 的 `main` 对其他 target 是排除的（`src/build/plan.cppm:882-895`），155 个各带 `main()` 的文件不会 `multiple definition`。实测 3 target 一次构建出 3 个二进制。

**✅ 扁平布局，零文件搬迁。** 不需要 `src/` 目录，`main` 可指向包根下任意相对路径：

```toml
[build]
sources = ["*.cpp"]

[targets.ex0]
kind = "bin"
main = "00-auto-and-decltype-0.cpp"
```
实测通过。dslings 现有目录结构可以原地不动。

**✅ 每 target 编译选项。** `cxxflags` / `cflags` / `defines` 可用。文档警告"只作用于该 target 的入口 TU"——但对"一个练习 = 一个 `.cpp`"的场景，入口 TU **就是**整个程序，所以 `-fno-elide-constructors`、`-Wpedantic -Werror`、`-O0` 全部覆盖到位。这三个特殊 target 无损迁移。

**✅ 非模块的普通 C++ 能构建。** `import std` 的 BMI 预构建是条件触发的（`src/build/prepare.cppm:3176-3217`），155 个无 `import` 的普通 `.cpp` 走快速路径。

**✅ 退出码符合 d2x 契约。** 实测：正常构建 0 / 编译失败 1 / 成员不存在 2 / `run` 0。

**✅ 速度显著优于 xmake。** 同机实测：

| 操作 | xmake | mcpp |
|---|---|---|
| 列出 target | **2.06 s** | — （无此命令） |
| 单练习构建（冷） | 0.63 s | **0.12 s** |
| 单练习构建（暖，无改动） | 0.61 s | **0.014 s** |

xmake 每次都要付 Lua VM 启动成本，暖构建几乎不省。mcpp 暖路径 14 ms —— 对 `d2x checker` 这种"改一行、立刻重编"的循环，这是量级差异。

**✅ 自带工具链沙箱。** 这正是 d2x 自己迁到 mcpp 时解决 macOS SDK 问题的关键。同时能一并解掉 CI 里那条已知的坑：d2x 的 `checker-smoke` job 注释写着 *"d2mcpp 的 .xlings.json 钉了 xmake 3.0.7，它已从 registry 移除，所以 xlings 的 xmake shim 会解析失败"*（`d2x/.github/workflows/ci.yml:124-134`）。迁到 mcpp 后这个 workaround 可以整个删掉。

**✅ Windows 覆盖更好。** MinGW（`x86_64-windows-gnu`）与 MSVC（`msvc@system`，0.0.90 起）都支持，且默认 `static_stdlib = true`——正好对上 d2x 会清空 `LD_LIBRARY_PATH` 的行为，比现在手写 `add_ldflags("-static")` 更稳。

### 2.2 阻塞点

**🚨 阻塞 1：C++ 标准下限是 c++23。**

```
$ mcpp build        # mcpp.toml 里 standard = "c++11"
error: unsupported C++ standard 'c++11'; expected c++23, c++26, c++2c,
       gnu++23, gnu++26, c++latest, or c++fly
```

白名单硬编码在 `mcpp/src/manifest/types.cppm:539-586`。所有绕行通道都被显式堵死：

```
$ # [targets.X].cxxflags = ["-std=c++11"]
error: targets.ex-c.cxxflags contains '-std=c++11'; use [package].standard
       to configure the C++ language standard
```
（`[build].cxxflags` 同样有此守卫，见 `toml.cppm:830-836`。）

对一个**以 C++ 标准分级为组织主轴**的课程仓库（`cpp11/` `cpp14/` `cpp17/` `cpp20/` `cpp23/`），这是根本性冲突。

**这有多严重？我做了量化实测**：把 51 个 `solutions/` 全部在"原标准"与 c++23 下编译并 diff 运行输出（地址已归一化）：

```
identical output : 50
DRIFTED          : 1   ← 04-rvalue-references
```

好消息：**50/51 无差异**，短期风险比预想的低。坏消息：**漂移的那一个恰好是移动语义的核心课**。C++11 下：

```
----> 临时对像 - 右值2
Object():0x...60c
Object(Object&&):0x...608     ← 移动构造，整节课的教学点
~Object():0x...60c
```

C++23 下（同样带 `-fno-elide-constructors`）：

```
----> 临时对像 - 右值2
Object():0x...de8             ← 只剩这一行
```

因为 C++17 的**保证复制省略**让 `-fno-elide-constructors` 无法再强制 prvalue 实质化。教学内容被标准升级**直接抹掉**，而不是"编译失败"这种能被 CI 发现的显式故障。随着课程往 cpp17/20 扩写，这类静默漂移只会变多。

顺带确认：`07-constexpr-0` 的 `-Wpedantic -Werror` 守卫（VLA 报错）在两个标准下**都成立**（各 3 个 error），这个练习无损。

**❌ 阻塞 2：`mcpp build <name>` 不能按 target 过滤。**

`build` 子命令**没有位置参数**（`src/cli.cppm:217-241`），传进去的名字被静默忽略，永远全量构建。实测：`rm -rf target && mcpp build ex-a` → 三个二进制全出来了。（`-p/--package` 选的是 workspace 成员，不是 target；`--target` 是**平台三元组**，不是 target 名。`mcpp run <name>` 倒是支持。）

这与 dslings 的语义直接冲突：**练习本来就该是编译不过的**（`D2X_YOUR_ANSWER` 宏展开为空，49 个练习带该标记）。实测单包场景：

```
$ mcpp build          # 3 个 target，其中 1 个坏
error: build failed
...
$ ls target/.../bin/  # 空
```

一个坏兄弟拖垮整包、且**一个产物都不出**。

**❌ 阻塞 3：没有列 target 的命令，更没有机器可读输出。**

全 CLI 22 个命令里没有 `list`/`targets`/`metadata`。唯一的 `--json` 是 `mcpp xpkg parse --json`。最接近的是 `target/.build_cache` 里的纯文本 `runTargets=<N>` 块。d2x 的 `list` 契约无法直接满足。

### 2.3 阻塞 2 有一个可用的逃生舱

mcpp 后端是 ninja，且 **`build.ninja` 在构建失败时依然会生成**。实测（`ex-b` 故意写坏，先 `mcpp build` 失败退出 1，再清空产物只留 `build.ninja`）：

```
1) mcpp build (all, b broken) exit=1
2) 清空 bin/ 和 obj/，保留 build.ninja
3) ninja bin/ex-a exit=0   → bin/ 里出现 ex-a
4) ninja bin/ex-c exit=0   → bin/ 里出现 ex-c
5) ninja bin/ex-b exit=1   （坏的那个，正确失败）
6) 最终 bin/: ex-a  ex-c
```

**逐 target 隔离 + 正确退出码，完全满足 d2x 契约。** `ninja -t targets` 还能直接枚举出 `bin/ex-a: cxx_link` 这样的列表，可以作为 `list` 命令的数据源之一。

代价：适配器要依赖 mcpp 的内部产物布局（`target/<triple>/<fingerprint>/build.ninja`，路径含哈希，需 `find` 定位）和一个 ninja 可执行文件。**这是耦合内部实现，属于技术债，应该同时向上游提需求。**

### 2.4 workspace 方案为什么不推荐

workspace 能天然隔离失败（实测 `mcpp build -p a` 在兄弟成员坏掉时正常返回 0），但代价过高：

- **members 不支持 glob**——`docs/06-workspace.md:34` 写了 `members = ["libs/*"]`，**实测报错** `workspace member 'a' not found in [workspace].members`。解析器逐字符串比较（`src/project.cppm:40`）。155 个练习要在根清单里手写 155 条字面路径。**这是文档 bug，值得单独给 mcpp 提 issue。**（另：`workspace.exclude` 被解析后**从未被读取**，是死配置。）
- 每个成员要独立 `mcpp.toml` + 独立 `target/` 目录 → 155 个清单、155 个构建目录。
- **workspace 根的 `[build]` 不被成员继承**——实测根上写 `include_dirs` 成员读不到，155 个成员每个都得重复写 `include_dirs = ["../../.."]`。
- **成员构建是串行的**（`src/cli/cmd_build.cppm:65-74` 是普通 `for` 循环，每个成员完整走一遍 `prepare_build` + spawn ninja）。

---

## 3. 结论与建议

### 3.1 直接回答

**"除去 xmake、用 mcpp 支撑整个项目"——今天做不到。** 卡在三点：C++ 标准下限、无按 target 构建、无 target 枚举。其中第 2、3 点有 workaround（ninja 直连），**第 1 点没有 workaround**，必须改 mcpp。

但三点的修复成本都不高，而且第 1 点的修复位置非常明确、局部。

### 3.2 推荐路线：单包多 target + ninja 直连，分三阶段

> ⚠️ **本节已被 §4（rustlings/cargo 对照）修订。** 布局从"单包 155 target"改为"**按标准分 workspace member**"，上游需求从 3 个降为 1 个必需 + 2 个可选。下面的阶段划分仍然成立，但请以 §4.4 的架构为准。

**阶段 0 — 先给 mcpp 提上游需求**（这是关键路径，其余都在等它）

| # | 需求 | 位置 | 估计改动 |
|---|---|---|---|
| 1 | **【必需】** `[package].standard` 放宽到 `c++11/14/17/20`（含 `gnu++` 变体） | `mcpp/src/manifest/types.cppm:520-586` 的 `normalize_cpp_standard` | ~30 行，纯新增分支。已确认 `[language].modules must be true` 的守卫（`toml.cppm:146-149`）**只在出现废弃的 `[language]` 段时才触发**（`had_language_section`），对源码无硬约束，不构成架构冲突 |
| 2 | 【可选】`mcpp build <target>` 按 target 名过滤（对齐已有的 `mcpp run <target>`） | `mcpp/src/cli.cppm:217-241` 加位置参数 + `src/build/execute.cppm` 过滤 link unit | 小。有 ninja 兜底 |
| 3 | 【可选】`mcpp list --json`（或 `mcpp targets`）输出 target 名 + 源文件列表 | 新增子命令，数据已在 plan 里 | 小。有 `ninja -t targets` 兜底 |

附带值得报的两个 bug：workspace `members` glob 文档与实现不符（cargo 同款语法可用，mcpp 报错）；`workspace.exclude` 是死代码。

> ❌ **不要提"per-target `standard`"这个需求。** 我一度打算这么提（对齐 cargo 的 per-`[[bin]]` `edition`），但实测发现 **cargo 正在废弃该字段**：`warning: 'edition' is set on binary 'ex-c' which is deprecated`。cargo 官方文档标注其"将在未来 Edition 移除"。语言版本的正确粒度是**包**，不是 target。详见 §4.3。

**阶段 1 — 在 d2mcpp 侧写 mcpp 适配器**（可与阶段 0 并行）

- 新建 `d2x/buildtools/mcpp/`，与现有 `xmake/` 平级（`cmake/README.md` 早就占了位，内容只有 `TODO`——说明多适配器一直在规划里）。
- 用生成器从 `dslings/**/*.cpp` 生成 `mcpp.toml`（155 个 `[targets.*]`，扁平布局，零文件搬迁）。生成器要处理的特殊情况只有 32 个 target、5 种模式，全部可枚举。
- 适配器实现四个命令：
  - `init` → `mcpp build`（忽略失败，目的是刷新 `build.ninja`）
  - `list` → `ninja -t targets` 或直接读生成器的映射表，输出 `name@ /abs/path`
  - `build <t>` → 上游需求 2 落地前用 `ninja -C <dir> bin/<t>`，落地后换成 `mcpp build <t>`
  - `run <t>` → `mcpp run <t>`
- 注意三个坑：**target 名要 shell 安全**（d2x 不做转义）；**路径必须绝对**；**名字里不能有 `@`**（会让整行被静默丢弃）。
- 命名要保持字典序 = 教学顺序（d2x 用 `std::map`）。

**阶段 2 — 切换与收尾**

- `.d2x.json` 的 `buildtools` 改指向 mcpp 适配器；`.xlings.json` 去掉 `xmake` 与 `gcc`/`mingw-w64` 的 pin，换成 `mcpp`（mcpp 自带工具链）。
- 删掉 `dslings/xmake.lua:1-5` 的 Windows 分支（mcpp 默认静态链接 + 自带 MinGW/MSVC 覆盖）。
- d2x 的 `checker-smoke` CI job 可以删掉"用真 xmake 而非 xlings shim"的 workaround。
- **`add_includedirs(".")` 对应 `[build] include_dirs = ["."]`** ——155/155 个文件全靠它，别漏。

### 3.3 阶段 0 完成前的过渡建议

- **不要现在就把 dslings 迁到 mcpp。** 强行迁移意味着把 cpp11 课程按 c++23 编译，`04-rvalue-references` 的教学内容会**静默失效**（不是编译报错，CI 抓不到）。
- **可以现在就用 mcpp 的地方**：未来的 `dslings/cpp23/`。那个目录现在是空占位，而 C++23 模块教学本来就该用 mcpp（`d2x/mcpp/common.cppm` 已经放了一个 `export module d2x.project.common;` 的桩）。以它为试点验证适配器，零迁移风险。
  - ⚠️ 但那个桩现在是坏的：`#include` 在全局模块片段里且什么都没 `export`，而**宏本来就不能跨模块导出**——`import d2x.project.common;` 拿不到 `d2x_assert` 也拿不到 `D2X_WAIT`，即拿不到 d2x 的整个判定协议。cpp23 章节要么继续 `#include` 那个头，要么把断言重新设计成非宏形式。这需要先决策。
- 注意 d2x 目前**只支持单一 `buildtools` 配置**。若要 cpp11-20 留 xmake、cpp23 用 mcpp，需要适配器内部按目录路由，或给 d2x 加多 buildtools 支持。**前者更简单，建议走前者。**

### 3.4 顺带发现的既有问题（与迁移无关，但建议一并修）

1. **`dslings-ref-ci.yml` 是个静默空转的 CI。** 它只挑 `-ref` 结尾的 target，而 `solutions/` 在 `xmake.lua:6` 被注释掉了 → grep 返回空 → 循环全跳过 → job 退出 0。第 60 行的 `wc -l` 对空串仍打印 "Found 1 reference targets"，把空转掩盖了。**参考答案实际上没有任何 CI 校验。**
2. **`dslings/en/cpp11/xmake.lua` 少一个 target**：`00-auto-and-decltype-5.cpp` 文件在但没有 target（49 文件 / 48 target）。
3. **命名不一致**：dslings 用 `cpp11-00-auto-and-decltype`（无 `-0`），solutions 用 `cpp11-00-auto-and-decltype-0-ref`。迁移生成器必须处理。
4. **`solutions/xmake.lua` 没有 Windows 分支**，若重新启用会在 Windows 上缺 UTF-8/gcc/静态链接处理。
5. **`d2x/src/checker.cppm:74-76` 无 `files.empty()` 保护**就索引 `files[0]`——某个 target 若列出零文件会崩。`d2x/docs/crash-analysis-d2x-in-d2mcpp.md:52` 声称加过保护，实际没有。

---

## 附：能力对照速查

| 需求 | xmake（现状） | mcpp 0.0.99 | 迁移后 |
|---|---|---|---|
| 155 个单文件可执行 | ✅ | ✅ `[targets.*]` | ✅ |
| 零文件搬迁（扁平布局） | ✅ | ✅ 实测 | ✅ |
| 每 target 编译选项 | ✅ | ✅ `cxxflags`/`defines` | ✅ |
| **每 target / 每目录 C++ 标准** | ✅ | 🚨 **下限 c++23，`-std=` 被硬拦** | 需上游 #1 |
| **只构建一个 target** | ✅ | ❌ 位置参数被忽略 | ninja 兜底 / 上游 #2 |
| **坏兄弟不拖垮好 target** | ✅ | ❌ 单包全挂且零产物 | ninja 兜底 |
| **枚举 target** | ✅ 插件 | ❌ 无命令、无 JSON | 生成器兜底 / 上游 #3 |
| 插件机制（`add_plugindirs`） | ✅ | ❌ 无 | 不需要，适配器可独立 |
| 单练习构建耗时（暖） | 0.61 s | **0.014 s** | 大幅改善 |
| 工具链自管理 | ❌ 依赖 xlings pin（3.0.7 已从 registry 移除） | ✅ 沙箱自带 | 大幅改善 |
| Windows | MinGW，需手写 `-static` | MinGW + MSVC，默认静态 | 改善 |

---

## 4. 横向对照：rustlings 和 cargo 是怎么做的

dslings 与 rustlings 是同构问题——~155 个单文件练习、**默认就编译不过**、由 checker CLI 驱动。rustlings 恰好在 2024 年做过一次同方向的架构迁移，其取舍和踩到的坑对我们有直接参考价值。

### 4.1 rustlings 的两代架构

**v5 及以前：直调 `rustc`，不用 cargo。**（`src/exercise.rs` @ tag `5.6.1`）

```rust
const RUSTC_EDITION_ARGS: &[&str] = &["--edition", "2021"];
const CLIPPY_CARGO_TOML_PATH: &str = "./exercises/clippy/Cargo.toml";

Mode::Compile => Command::new("rustc")
    .args([self.path.to_str().unwrap(), "-o", &temp_file()])
    .args(RUSTC_COLOR_ARGS).args(RUSTC_EDITION_ARGS).output(),
```

产物是 CWD 下 pid+线程 id 命名的临时文件，靠 `impl Drop for FileHandle` 回收。IDE 支持靠**另一套**手写生成的 `rust-project.json`（`src/project.rs`，需要手动跑 `rustlings lsp`）。clippy 则再套一层 hack：运行时往固定路径写一个临时 `Cargo.toml`、`cargo clean`、`cargo clippy`——每次全量重建，且练习被编译两遍。

**两套真相源直接酿成 bug。** PR #1355 记录得很清楚：

> The edition args of `rustc` is set to 2018 by default. While rustling generate `rust-project.json` with edition setting to 2021. It may cause some problems.

**v6（2024-03-31，commit `82b563f1`，`src/exercise.rs +56/−204`）：生成 `Cargo.toml`，改用 cargo。** 动机在 issue #1935 里说得很直白，**三条理由里没有一条是速度**：

> We can generate a `Cargo.toml` file containing a `[[bin]]` entry for every exercise. This should make things much smoother and less hacky, especially because we have many issues related to the **language server**. This should also enable us to use Cargo instead of rustc to run the exercises. That would especially make **Clippy** exercises less of a trouble. This approach also allows us to easily add **dependencies** to the exercises in case we want to do so in the future.

即：**LSP、Clippy、未来的依赖管理**——外加消灭第二套真相源。

### 4.2 v6 的具体机制

| 项 | rustlings 做法 |
|---|---|
| 清单 | `dev/Cargo.toml` 是模板，**生成且提交进仓库**（不 gitignore）。只有 `bin = [...]` 那一段被机器重写（`src/cargo_toml.rs::bins_start_end_ind` 按字节偏移做字符串手术），其余手写内容原样保留，学习者可自行加 `[dependencies]` |
| 何时重新生成 | 只在显式 `rustlings dev update`，**从不在运行时隐式生成**。CI 用 `dev check` 重新生成并逐字节比对，不一致就报 "Cargo.toml is outdated" |
| 条目形式 | 不是 `[[bin]]` 段头，是等价的内联表数组，**只有 `name` 和 `path` 两个键**：`{ name = "intro1", path = "../exercises/00_intro/intro1.rs" }`。必须放在任何 table 头之前 |
| 规模 | 94 个练习 → **188 个 bin**（每个练习额外生成一个 `<name>_sol` 指向参考答案） |
| 构建命令 | `cargo build -q --bin <n>` → 失败即止；`cargo test -q --bin <n>`；`cargo clippy -q --bin <n> --profile test`；最后**直接 exec `target/debug/<n>`，不用 `cargo run`** |
| 诊断格式 | **纯文本**，不用 `--message-format=json`。`cargo metadata` 只用来取 `target_directory` |
| 练习清单来源 | `rustlings-macros/info.toml`，proc macro 在编译期 `include_bytes!` 进二进制。字段：`name` / `dir` / `test`(默认真) / `strict_clippy` / `hint` / `skip_check_unsolved` |
| 通过判定 | **纯退出码**，无输出匹配。特别地，为了避免练习里裸 `exit(1)` 让人一头雾水，会补一句 "The exercise didn't run successfully (nonzero exit code)" |
| 完成状态 | `.rustlings-state.txt`（gitignore）。格式极简：第 3 行是当前练习名，第 5 行起是已完成练习名。**只缓存"在哪一题"和"哪些做完了"**，不缓存编译输出。用名字不用索引，所以重排 `info.toml` 不会毁进度 |
| 文件监听 | `notify` v8 + **200ms 去抖**（脏位图，N 次快速保存塌缩成 1 次重建）+ `EXERCISE_RUNNING: AtomicBool` 自触发保护 + 激进事件过滤（只放行 `Modify::*` 和 `Access::Close(Write)`） |

`// I AM NOT DONE` 标记**在 v6 已被删除**，改为按 `n` 显式进入下一题 + 持久化状态。

### 4.3 cargo 的能力（我在本机 cargo 1.89 实测）

做了和 mcpp 完全对等的实验（3 个 bin，`ex-b` 故意写坏）：

| 能力 | 结果 |
|---|---|
| `cargo build --bin ex-a` | ✅ exit 0，只产出 `ex-a`，坏兄弟完全无关 |
| `cargo build --bin ex-b` | ✅ exit 101 |
| `cargo run --bin ex-a` | ✅ 且**透传程序退出码**（实测 `exit(3)` → 101 之外的 3） |
| `cargo metadata --format-version 1 --no-deps` | ✅ JSON 含 name + **绝对 `src_path`** + edition；**练习编译不过时照样 exit 0**（清单层查询，不编译） |
| workspace `members = ["ex/*"]` glob | ✅ 可用（mcpp 文档写了但实测报错） |
| `--message-format=json` | ✅ 给出 `file:line:col` 结构化诊断 |
| 逐 bin 构建耗时 | 暖 0.055s / 改动后 0.136s / `metadata` 0.038s |

**🚨 但 per-`[[bin]]` `edition` 是个陷阱。** 它确实能工作（实测包级 2021 + 单 bin 覆盖 2015，2015-only 代码正确通过/拒绝），**但 cargo 会警告并正在移除**：

```
warning: `edition` is set on binary `ex-c` which is deprecated
```

cargo book 标注该字段 *"deprecated and will be removed in a future Edition"*。而 rustlings **从未使用过它**——全仓库 grep `edition` 只有一个值 `2024`，且从不出现在 bin 条目里；`append_bins()` 在代码层面就只能写 `name` 和 `path`；`ExerciseInfo` 没有任何 edition/flags 字段。188 个 target 全部继承唯一的包级 `edition = "2024"`。

> 而且即便只有一个包级 edition，rustlings 也已经踩到漂移风险——`Cargo.toml:17` 挂着注释 `# On Update: Update the edition of rustfmt in dev check and CARGO_TOML in dev new`，同一个值手工同步三处。

**结论：语言版本的正确粒度是「包」，不是「target」。** 这直接否定了我原先"给 mcpp 加 `[targets.X].standard`"的想法。

### 4.4 修订后的推荐架构：按标准分 workspace member

既然粒度是包，而 **d2mcpp 的目录结构本来就是按标准组织的**，两者天然对齐：

```
mcpp.toml                    [workspace] members = ["dslings/cpp11", "dslings/cpp14", ...]
dslings/cpp11/mcpp.toml      standard = "c++11"   + 49 个 [targets.*]
dslings/cpp14/mcpp.toml      standard = "c++14"   +  2 个
dslings/cpp17/mcpp.toml      standard = "c++17"
dslings/cpp20/mcpp.toml      standard = "c++20"
dslings/cpp23/mcpp.toml      standard = "c++23"
```

**5~10 个 member，不是 155 个。** §2.4 里反对 workspace 的四条理由全部失效：members 不用 glob（10 条字面路径无所谓）、10 个 `mcpp.toml` 不是负担、`include_dirs` 重复 10 次不是负担、串行构建 10 个 member 也不是负担。

已实测验证（用 c++23/c++26 两个 member，因为 c++11 目前仍被拒）：

```
cpp23 member -> ['-std=c++23']      ← 各自 compile_commands.json
cpp26 member -> ['-std=c++26']
```
**每 member 的 `standard` 正确落到编译行。** 且 `mcpp build -p cpp26` 在 `cpp23` member 有坏练习时正常返回 0。

剩余问题只有两个，都已有解：
- **member 内部**仍需逐 target 隔离（一个坏练习会拖垮同 member 的 49 个）→ ninja 兜底（§2.3 已实测）或上游需求 2。
- `08-literal-type-0` 需要 c++17 但住在 cpp11 目录 → 单独开一个小 member，或挪到 cpp17。

**上游需求从 3 个降为 1 个必需：只剩 `[package].standard` 放宽。** 而且这个方向与 cargo 的包粒度哲学一致，不是在跟工具较劲。

### 4.5 rustlings 踩过的坑：哪些我们会中、哪些不会

**⚠️ 会中的：性能是这个模型引入的**回归**，不是收益。** issue #2071「Checking an exercise takes too long」（2024-08 提出，**至今未关**）：

> every time I update the code of an exercise, the `rust-analyzer` builds the whole `exercises` folder (it has like 156 exercises...). During this process, I cannot use any command.

维护者承认 checker 和 IDE 争抢同一个 cargo lock 与 target 目录，且保存一次就全量失效重算。连带 #2113「Rustling is slow」、#2091「Too many open files (os error 24)」、#2072「LSP issues because of too many errors」。至今只有缓解措施（`rust-analyzer.cargo.targetDir` 隔离目录）。

**✅ 但我们大概率不会中最严重的那条**——因为这本质是 **rust-analyzer 特有的全 crate 检查模型**。C++ 侧是 clangd + `compile_commands.json`，**逐 TU 按需**，第 90 个练习编译不过不影响第 3 个的诊断。我实测确认 mcpp 的行为正好支持这点：

- **构建失败时 `compile_commands.json` 照样生成**（104-target spike：`mcpp build` 退出 1，但 104 条 entry 齐全）
- 每条 entry 带自己的 `-std=`，配合 §4.4 的分 member 架构，clangd 能对每个练习显示**正确标准**下的诊断

**这反而是迁移的一个净收益。** d2mcpp 现在**没有** `compile_commands.json`（`.gitignore:49` 预留了位置但文件不存在），IDE 靠 `.vscode/settings.json` 里手写的 `C_Cpp.default.includePath`——它不编码任何 `-std=`，等于**全仓库一个标准**。这正是 rustlings PR #1355 那类"两套真相源"bug 的同款形态。

**✅ 独立于构建系统的、更便宜的一个win：done-state 缓存。** rustlings 的"每次重编所有练习"抱怨（#121/#132/#1843）**早于 v6，且是靠 `.rustlings-state.txt` 解决的，不是靠换 cargo**。d2x 目前没有任何完成状态持久化。这件事现在就能做，与 xmake/mcpp 之争无关。

**✅ 值得直接抄的 CI 门禁。** rustlings 的 `cargo dev check --require-solutions` 断言：**每个练习都必须失败，每个参考答案都必须通过**。对照 §3.4 第 1 条——d2mcpp 的 `dslings-ref-ci.yml` 目前是静默空转，参考答案零校验。这个门禁应该优先补上。

**✅ 值得抄的两个防御细节：**
- rustlings 全量检查时，**"报错"（而非"失败"）的练习会退回串行重试**，注释写明 *"it could be because we exceeded the limit of open file descriptors"*。155 个 target 并发时同样会遇到。
- 文件监听必须有**自触发保护**（构建自身的写入不能反过来触发重建）+ **去抖**。d2x 现在是 `wait_files_changed` 轮询 mtime，没有这两层。

### 4.6 一句话总结

rustlings 的证据**支持**"生成单一构建描述、一个练习一个 target"这条路线，但**理由是 IDE/lint 集成和消灭第二套真相源，不是速度**。语言版本用**包**粒度（cargo 正在废弃 target 粒度），这恰好让 d2mcpp 的"按标准分目录"结构变成天然的 workspace member 划分，把 mcpp 的上游需求压缩到只剩一条。
