# d2mcpp「练习即测试」迁移 — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 按 `2026-07-23-exercises-as-tests-design.md` 完成 d2mcpp 侧迁移：无宏 harness、`cpp*/tests/` 目录、Provider 瘦身为 `mcpp test --message-format json` 的薄壳，并用本地 musl mcpp 二进制端到端打通 `d2x checker`。

**Architecture:** 试点先行——先迁 cpp14（2 题）验证整条链（harness 模块导入、mcpp test 命名/隔离/JSON、Provider 判定合并、d2x checker），再脚本化批量迁移 cpp11×49 + en×52 + solutions×51。mcpp 依赖本地分支 `feat/test-isolation-json` 的 musl 静态二进制（`ls -dt mcpp/target/x86_64-linux-musl/*/bin/mcpp | head -1`），通过 PATH 前置注入。

**Tech Stack:** C++23 modules（harness/练习）、C++26（Provider）、bash（迁移脚本 + e2e）。

## Global Constraints

- 分支:`feat/mcpp-provider`(d2mcpp)、`feat/exercise-framework-protocol`(d2x),均不推送,本地交付。
- musl mcpp 注入方式:`export PATH="$SHIM:$PATH"`,SHIM 目录里只有 mcpp 符号链接。
- 练习可见输出格式(`[HONLY LOGI]: - ✅ | ...`)**保持不变**——book 里引用了它,格式漂移=文档漂移。
- 侧信道 v2:每行加 `"v":2`;assert 增可选 `"what"`;判定顺序与设计稿 §5 一致。
- id/order/chapter 推导公式不变(`cpp11-00-auto-and-decltype-0`,order=rank×100000+章×100+序)。
- Provider 协议事件(describe/exercise/stage/output/verdict/error)一个字段都不改——d2x 零改动是设计承诺。
- e2e 两道防线(pass==0 防空转、脏树拒绝)原样保留。

## Tasks

- [x] **T1 harness/ 新包**:顶层 `harness/`,纯模块 `d2x.harness`(check/check_eq/wait,source_location,`what` 可选参),atexit 退出码(失败或 wait 未拆→`_Exit(1)`),侧信道 v2 内联进模块(report.hpp 内容并入,include 路径退役)。单测:临时 fixture 直接 mcpp test。
- [x] **T2 试点 cpp14**:`cpp14/mcpp.toml`(dep harness path)+ `cpp14/tests/00-generic-lambdas/{0,1}.cpp`(内容转换:include→import,宏→函数)+ solutions 镜像;根 workspace 更新;`mcpp test -p cpp14` 原生验证(未完成→fail,答案→pass)。
- [x] **T3 Provider 瘦身**:discovery 扫 `cpp*/tests`+`intro/tests`(en 前缀 `en/`);check = spawn `mcpp test <name> --message-format json -p <member>` + 读侧信道 + 合并判定;manifest.cppm 删除;runner.cppm 换成 mcpp-test 解析器;unsetenv workaround 删(mcpp c1bf311 已根治)。
- [x] **T4 批量迁移**:脚本迁 cpp11×49 + en/×52 + intro(hello-mcpp)+ solutions;两道特殊 cxxflags 题改用 `[build] flags = [{glob=...}]`(先验证 glob 对 tests/ 生效,不生效则保留头部指令+Provider 透传······不,tests 由 mcpp 驱动,必须 toml 承载,验证是硬前提);删 dslings/。
- [x] **T5 e2e.sh 重写**:两次全量 `mcpp test`(pristine 全 fail + overlay 全 pass)×zh/en + Provider 协议冒烟(枚举 52、check 单题事件流);hello-mcpp 补答案入 e2e。
- [x] **T6 d2x 端到端**:构建 d2x(feat/exercise-framework-protocol,musl mcpp on PATH),`d2x checker` 跑通新链路;.d2x.json 不变。
- [x] **T7 文档收尾**:book 路径 sed、authoring skill、CI workflow(注明需 mcpp 下一发版)、参考文档更新记录、CHANGELOG 式提交。
