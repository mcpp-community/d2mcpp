# msvc-stl

> 🌎 [English](#english) | [中文](#中文)

---

## English

A **read-only reference copy** of Microsoft's open-source STL implementation. It backs the *"Real-world Case"* sections across the d2mcpp chapters, used to corroborate how language features are actually used in production-grade code. **This is NOT this repository's own code — do not modify it.**

### Upstream

- Repository: <https://github.com/microsoft/STL>
- License: Apache-2.0 WITH LLVM-exception (see `LICENSE.txt` / `NOTICE.txt`, kept verbatim)
- Snapshot commit: `926d9b2b1f75c6b0cdc7fe2d65630f9a81825c05`
- Commit date: 2026-05-28

### Scope

Only the complete library subtree `stl/` is kept (`inc/` + `src/` + `modules/` + its own build files). Excluded:

- `tests/`, `benchmarks/`, `tools/` — tests / benchmarks / tooling, not the library implementation
- `.git/` — upstream history
- `llvm-project`, `boost-math`, `benchmarks/google-benchmark` — submodules that only serve testing

### Refresh

```sh
# 1. Clone upstream (shallow is fine)
git clone --depth 1 https://github.com/microsoft/STL.git /tmp/STL

# 2. Replace the stl/ subtree and sync the licenses
rm -rf msvc-stl/stl
cp -r /tmp/STL/stl        msvc-stl/stl
cp    /tmp/STL/LICENSE.txt msvc-stl/LICENSE.txt
cp    /tmp/STL/NOTICE.txt  msvc-stl/NOTICE.txt

# 3. Update the commit / date in this file, and re-check that line
#    numbers referenced by each chapter still match
```

---

## 中文

微软开源 STL 实现的**只读引用基底**,供 d2mcpp 各章「真实案例」节链接、印证语言特性在生产级代码中的真实用法。**非本仓库自有代码,请勿修改。**

### 上游来源

- 仓库:<https://github.com/microsoft/STL>
- 许可:Apache-2.0 WITH LLVM-exception(见 `LICENSE.txt` / `NOTICE.txt`,原样保留)
- 快照 commit:`926d9b2b1f75c6b0cdc7fe2d65630f9a81825c05`
- commit 日期:2026-05-28

### 包含范围

仅保留完整的库子树 `stl/`(`inc/` + `src/` + `modules/` + 其自带构建文件),已排除:

- `tests/`、`benchmarks/`、`tools/`(测试 / 基准 / 工具,非库实现)
- `.git/`(上游历史)
- `llvm-project`、`boost-math`、`benchmarks/google-benchmark`(仅服务测试的子模块)

### 刷新方式

```sh
# 1. 克隆上游(浅克隆即可)
git clone --depth 1 https://github.com/microsoft/STL.git /tmp/STL

# 2. 用新的 stl/ 子树覆盖,并同步 license
rm -rf msvc-stl/stl
cp -r /tmp/STL/stl        msvc-stl/stl
cp    /tmp/STL/LICENSE.txt msvc-stl/LICENSE.txt
cp    /tmp/STL/NOTICE.txt  msvc-stl/NOTICE.txt

# 3. 更新本文件的 commit / 日期,并核对各章引用的行号是否仍对应
```
