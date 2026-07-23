# 练习即测试 —— 迁移后参考

- 日期：2026-07-23
- 分支：`feat/mcpp-provider`
- 设计稿：[`2026-07-23-exercises-as-tests-design.md`](2026-07-23-exercises-as-tests-design.md)
- 取代：[`2026-07-20-mcpp-provider-reference.md`](2026-07-20-mcpp-provider-reference.md) 中的清单生成（§4）与判定机制（§5）章节
- 本文定位：**迁移完成后的现状参考** —— 布局、判定链路、验证结果、依赖版本

---

## 1. 布局（第二轮重排后：库入驻 d2x/、课程收进 src/）

```
mcpp.toml                 workspace = [d2x, src/*, src/en/*, d2x/buildtools]
d2x/                      d2x 练习库(纯模块 `import d2x;`,零宏)
d2x/buildtools/      Provider(C++26)
src/intro/tests/hello-mcpp.cpp
src/cpp11/tests/<NN-topic>/<K>.cpp    49 题
src/cpp14/tests/00-generic-lambdas/{0,1}.cpp
src/en/{intro,cpp11,cpp14}/           en 镜像工程(lang=en 时启用,与 zh 互斥)
solutions/<std>/<NN-topic>/<K>.cpp    52 份(zh/en 共用,含 hello-mcpp)
```

- **零生成物**：`.d2x/build/` 及三套清单生成已删除；`.d2x/` 下只剩学习进度
  `state.json`（d2x 写）与判定侧信道 `result.ndjson`（harness 写）。
- id 推导与旧布局**逐字节一致**（`cpp11-00-auto-and-decltype-0`），学习者进度不丢。

## 2. 双入口

```
mcpp test -p src/cpp11              原生:进度表(-p cpp11 的 basename 简写同样可用)
mcpp test -p src/cpp11 00-auto      原生:按子串过滤(匹配整章;更长的名字如
                                    00-auto-and-decltype/3 即单题)
d2x checker                         闯关:文件监听、通过自动推进(已实测:改对
                                    hello-mcpp 后自动进入 cpp11-00-auto-and-decltype-0)
```

同一条链路：`d2x checker → Provider → mcpp test --message-format json → 判定`；
Provider 只是把 mcpp 的 JSON 记录 × 侧信道合并成协议 verdict。

## 3. d2x 库（`import d2x;`，包与模块同名，住在 `d2x/`）

| 设施 | 说明 |
|---|---|
| `d2x::check(cond, "原文")` / `check_eq(a, b, "a == b")` | 第三参是给学习者看的表达式原文（迁移脚本自动从宏参数生成，教学输出零漂移）；`std::formattable` 探测转印值（SFINAE 安全，requires 里直接写 std::format 会踩未特化 formatter 的 static_assert） |
| `D2X_YOUR_ANSWER` | **纯约定，无定义**。裸标识符的编译报错正好指着要填的位置 |
| `d2x::wait()` | 路障；记录后继续执行（后续检查点照跑） |
| `d2x::dont_delete_this(expr)` | 恒等透传（旧 D2X_DONT_DELETE_THIS） |
| 退出码 | 首次调用注册 atexit：有失败检查点或未拆 wait → `_Exit(1)`。裸 `mcpp test` 因此天然能判对错 |
| 侧信道 v2 | `{"v":2,"kind":"assert","ok":…,"what":…,"expected":…,"actual":…,"file":…,"line":…}` / `{"v":2,"kind":"wait",…}`；`D2X_RESULT_FILE` 未设置时不写文件 |

**可见输出（去 HONLY 后的日志标识——emoji 即标识，无框架前缀）**：

```
✅ | a == a1 (1 == 1)                                          绿
❌ | a == b (1 == 2)  --> src/cpp11/tests/.../0.cpp:38          红
🚧 | Delete the d2x::wait() to continue  --> <file>:<line>      黄
```

失败/路障行带定位;路径经 cwd 剥离尽量以仓库相对形式展示（定位用的
绝对路径仍走侧信道，展示归展示、定位归定位）。

**判定顺序（注意与设计稿 §5 的一处刻意调整）**：失败断言 → fail；`wait` → blocked；
退出码非 0 → fail；无侧信道 → 退出码 0 即 pass。`wait` 判定**先于**退出码——
新 harness 里 wait 会把退出码顶成 1，按旧顺序 blocked 会全部误判成 fail。

**教学细节**：`import std` 不带宏——需要 `NULL` 的课（12-nullptr）显式
`#include <cstddef>` 并注释原因；全局 `int8_t` 等别名要写 `std::int8_t`。

## 4. 依赖的 mcpp 能力（本地分支 feat/test-isolation-json）

逐测试编译隔离（Phase A 包级/Phase B 每测试独立 ninja goal）、`mcpp test <pattern>`、
`--message-format json`、tests 相对路径命名、`[build].flags` glob 覆盖测试 TU、
`merged_environ` 剥离私有 glibc loader 路径（嵌套段错误根治，d2mcpp `runner.cppm`
的 `unsetenv` workaround 已删；d2x `platform.cppm` 的保留——它还保护 editor 等
非 mcpp 子进程）。**mcpp 发版并更新 xlings pin 前，CI 会失败，属预期。**

## 5. 验证结果（2026-07-23，musl 静态 mcpp）

| 项 | 结果 |
|---|---|
| e2e（`d2x/buildtools/tests/e2e.sh all`） | zh 52/52、en 52/52 答案全绿；pristine 双向 0-pass；Provider 冒烟 ✓ |
| d2x 端到端 | checker 显示 0/52 → 改对自动推进 → state.json 兼容旧 id ✓ |
| d2x 仓库改动 | **零**（协议边界承诺兑现） |
| hello-mcpp | 答案已补，进入 e2e，不再 SKIP |

## 6. 已知缺口（本轮后仍开）

- macOS / Windows 未实测（Provider 的 `_popen` 分支、mcpp test 三能力的 Windows 路径）
- 编译错误未结构化成协议 diagnostics（原始文本透传，独立增量）
- Provider 无单测（discovery 推导/JSON 解析仅被 e2e 间接覆盖）
- cpp17/20/23 目录尚无练习（撰稿侧工作，脚手架已就绪）
