#!/usr/bin/env bash
# d2mcpp 端到端验证 —— 两条硬断言,缺一不可:
#
#   1) 每个练习未完成时不通过 (pristine 全量 mcpp test 必须 0 passed)
#   2) 每份参考答案放进去后通过 (overlay 后全量 mcpp test 必须全绿)
#
# 这是 rustlings `cargo dev check --require-solutions` 的等价物。
#
# 两道防线(均源自历史缺陷,背景见 .agents/docs 参考文档):
#   - 脏树检查: 练习/答案目录有未提交改动时拒绝运行 —— 脚本要把答案覆盖到
#     练习上再用 git 还原,未提交的改动会被覆盖丢失
#   - 防空转: overlay 后 passed 总数为 0 时直接失败,绝不静默绿灯
#
# 用法:  bash d2x/buildtools/tests/e2e.sh [zh|en|all]   (默认 all)
# 依赖:  PATH 里的 mcpp 需支持 mcpp test 逐测试隔离/过滤/--message-format json
set -uo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
cd "$ROOT"

MODE="${1:-all}"

run_lang() {
    local prefix="$1"    # "src/" (zh) 或 "src/en/"
    local label="$2"
    local members=("${prefix}intro" "${prefix}cpp11" "${prefix}cpp14")
    local test_dirs=()
    for m in "${members[@]}"; do test_dirs+=("$m/tests"); done

    # —— 防线 1: 脏树检查 ——
    local dirty
    dirty=$(git status --porcelain -- "${test_dirs[@]}" solutions/ | head -20)
    if [[ -n "$dirty" ]]; then
        echo "E2E($label) 拒绝运行: 练习/答案目录存在未提交改动(脚本的覆盖-还原流程会将其覆盖丢失):"
        echo "$dirty"
        return 1
    fi

    restore() { git checkout -q -- "${test_dirs[@]}"; }

    # —— 断言 1: pristine 全量必须 0 passed ——
    local m out passed
    for m in "${members[@]}"; do
        out=$(mcpp test -p "$m" 2>&1)
        passed=$(echo "$out" | grep -oE '[0-9]+ passed' | grep -oE '^[0-9]+' | tail -1)
        if [[ "${passed:-0}" != "0" ]]; then
            echo "E2E($label) FAIL: $m 在未完成状态下有 $passed 个练习通过了 —— 练习失去了「默认不通过」性质"
            echo "$out" | grep " ... ok" | head -5
            return 1
        fi
    done
    echo "E2E($label) pristine: 全部练习保持未通过 ✓"

    # —— 覆盖参考答案 (zh/en 共用同一份 solutions) ——
    local n_overlaid=0 sol rel std rest dst
    while IFS= read -r sol; do
        rel="${sol#solutions/}"                      # intro/hello-mcpp.cpp | cpp11/00-x/0.cpp
        std="${rel%%/*}"; rest="${rel#*/}"
        dst="${prefix}${std}/tests/${rest}"
        if [[ ! -f "$dst" ]]; then
            echo "E2E($label) FAIL: 答案 $sol 没有对应的练习文件 $dst"
            restore
            return 1
        fi
        cp "$sol" "$dst"
        n_overlaid=$((n_overlaid + 1))
    done < <(find solutions -name '*.cpp' | sort)

    # —— 断言 2 + 防线 2: overlay 后全绿,且 passed 总数与答案数一致 ——
    local total_passed=0 ok=1
    for m in "${members[@]}"; do
        out=$(mcpp test -p "$m" 2>&1)
        if ! echo "$out" | grep -q "test result ok"; then
            echo "E2E($label) FAIL: $m 覆盖答案后仍有失败:"
            echo "$out" | grep -E "FAIL|failures" -A10 | head -15
            ok=0
        fi
        passed=$(echo "$out" | grep -oE '[0-9]+ passed' | grep -oE '^[0-9]+' | tail -1)
        total_passed=$((total_passed + ${passed:-0}))
    done
    restore

    if [[ "$ok" != 1 ]]; then return 1; fi
    if [[ "$total_passed" -eq 0 ]]; then
        echo "E2E($label) FAIL: 防空转 —— passed 总数为 0,脚本没有真正跑到任何练习"
        return 1
    fi
    if [[ "$total_passed" -ne "$n_overlaid" ]]; then
        echo "E2E($label) FAIL: 覆盖了 $n_overlaid 份答案但只有 $total_passed 个练习通过"
        return 1
    fi
    echo "E2E($label) solutions: $total_passed/$n_overlaid 参考答案全部通过 ✓"
}

provider_smoke() {
    # Provider 协议冒烟:枚举数量、check 的关键事件
    local n
    n=$(mcpp run -q -p d2x/buildtools -- exercises | grep -c '"event":"exercise"')
    if [[ "$n" -lt 52 ]]; then
        echo "E2E(provider) FAIL: 枚举到 $n 个练习(预期 >= 52)"
        return 1
    fi
    local out
    out=$(mcpp run -q -p d2x/buildtools -- check hello-mcpp)
    echo "$out" | grep -q '"event":"stage","name":"compile"' \
        && echo "$out" | grep -q '"event":"verdict"' \
        && echo "$out" | grep -q '"outcome":"fail"' \
        || { echo "E2E(provider) FAIL: check hello-mcpp 的事件流不完整:"; echo "$out" | head -3; return 1; }
    echo "E2E(provider) 协议冒烟: $n 个练习,check 事件流完整 ✓"
}

rc=0
provider_smoke || rc=1
if [[ "$MODE" == "zh" || "$MODE" == "all" ]]; then run_lang "src/"    zh || rc=1; fi
if [[ "$MODE" == "en" || "$MODE" == "all" ]]; then run_lang "src/en/" en || rc=1; fi

if [[ "$rc" == 0 ]]; then echo "E2E: ALL GREEN"; else echo "E2E: FAILED"; fi
exit "$rc"
