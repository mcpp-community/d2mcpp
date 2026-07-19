#!/usr/bin/env bash
# Provider 端到端测试。
#
# 断言两件事，缺一不可：
#   1. 每个练习「未完成时」不通过   —— 否则学员会被直接跳过，练习形同虚设
#   2. 每个参考答案「放进去后」通过 —— 否则参考答案本身是错的
#
# 这是 rustlings `cargo dev check --require-solutions` 的等价物。d2mcpp 现有的
# dslings-ref-ci.yml 因为 solutions/ 在 xmake.lua 里被注释掉，实际校验零个目标。
set -uo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../.." && pwd)"
cd "$REPO_ROOT"

PROVIDER=(mcpp run -q -p d2x/buildtools/mcpp --)

# 任何退出路径都还原被改动的练习文件，绝不把仓库留在脏状态。
#
# 只还原练习源文件，绝不整目录还原 dslings/ —— 脚手架库 dslings/harness/
# 也在这个目录下，整目录还原会把开发中的改动一起抹掉（踩过一次）。
EXERCISE_DIRS=(dslings/cpp* dslings/en dslings/hello-mcpp.cpp)
restore() {
    for d in "${EXERCISE_DIRS[@]}"; do
        [ -e "$d" ] && git checkout -- "$d" 2>/dev/null || true
    done
}
trap restore EXIT

outcome_of() {   # $1 = exercise id
    "${PROVIDER[@]}" check "$1" 2>&1 \
        | grep -o '"outcome":"[a-z]*"' | head -1 | cut -d'"' -f4
}

echo "==> 枚举练习"
mapfile -t LINES < <("${PROVIDER[@]}" exercises 2>&1 | grep '"event":"exercise"')
if [ "${#LINES[@]}" -eq 0 ]; then
    echo "FAIL: Provider 没有枚举出任何练习"; exit 1
fi
echo "    共 ${#LINES[@]} 个"

pass=0; fail=0; skipped=0

for line in "${LINES[@]}"; do
    id=$(printf '%s' "$line"   | grep -o '"id":"[^"]*"'      | head -1 | cut -d'"' -f4)
    file=$(printf '%s' "$line" | grep -o '"files":\["[^"]*"' | head -1 | cut -d'"' -f4)

    rel="${file#"$REPO_ROOT"/}"                 # dslings/cpp11/xx.cpp
    sol="solutions/${rel#dslings/}"             # solutions/cpp11/xx.cpp
    sol="${sol#en/}"                            # en 目录共用同一份参考答案

    # 1) 未完成态必须不通过
    got=$(outcome_of "$id")
    if [ "$got" = "pass" ]; then
        echo "FAIL [$id] 练习未完成却判定通过"; fail=$((fail+1)); continue
    fi

    # 2) 参考答案必须通过
    if [ ! -f "$sol" ]; then
        echo "SKIP [$id] 无参考答案 ($sol)"; skipped=$((skipped+1)); continue
    fi
    cp "$sol" "$file"
    got=$(outcome_of "$id")
    git checkout -- "$file"

    if [ "$got" = "pass" ]; then
        pass=$((pass+1))
    else
        echo "FAIL [$id] 参考答案未通过 (outcome=$got)"; fail=$((fail+1))
    fi
done

echo
echo "==> 参考答案通过 $pass · 失败 $fail · 跳过 $skipped"
[ "$fail" -eq 0 ]
