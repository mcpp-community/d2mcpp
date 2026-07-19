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

# 本脚本会把参考答案覆盖到练习上再还原，所以运行前练习必须是干净的 ——
# 否则未提交的改动会被 restore 悄悄丢掉（作者踩过两次：一次丢了脚手架，
# 一次丢了刚修好的练习）。宁可拒绝运行，也不能吃掉别人的工作。
if ! git diff --quiet -- "${EXERCISE_DIRS[@]}" 2>/dev/null; then
    echo "拒绝运行：练习目录有未提交的改动，本测试会在结束时还原它们。"
    echo "请先提交或 stash："
    git diff --stat -- "${EXERCISE_DIRS[@]}" | sed 's/^/  /'
    exit 2
fi

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

    # en/ 必须先剥，再拼 solutions/ —— 顺序反了的话 sol 已经以 "solutions/"
    # 开头，`${sol#en/}` 匹配不到任何东西，是个静默 no-op，结果所有英文练习
    # 都因为找不到 solutions/en/... 而被 SKIP，测试全绿却一个都没验。
    # 这正是本脚本头部注释里说要防的那种「空转」。
    rel="${file#"$REPO_ROOT"/}"                 # dslings[/en]/cpp11/xx.cpp
    rel="${rel#dslings/en/}"                    # en 镜像共用同一份参考答案
    rel="${rel#dslings/}"                       # cpp11/xx.cpp
    sol="solutions/${rel}"                      # solutions/cpp11/xx.cpp

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

# 防空转：一个参考答案都没验到时必须红，而不是「0 失败」蒙混过关。
# 旧 CI 就是这么绿了很久的 —— 它只挑 -ref 目标，而 solutions/ 早被注释掉，
# 循环一次都没进，job 照样退出 0。
if [ "$pass" -eq 0 ]; then
    echo "FAIL: 没有验证到任何参考答案 —— 测试本身失效了，不是「全部通过」"
    exit 1
fi

[ "$fail" -eq 0 ]
