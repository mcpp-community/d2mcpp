#!/usr/bin/env bash
# d2x checker 端到端验证 —— 一条硬断言:
#
#   把 solutions/ 全部覆盖到练习上之后, `d2x checker` 必须自己走完全部练习
#   并主动退出(退出码 0)。checker 只在「全部练习已完成」时才退出,平时它
#   会一直等文件改动,所以「正常退出」本身就是「全过」的判据。
#
# 为什么值得和 e2e.sh 分开:e2e.sh 只验 `mcpp test` 那一层。checker 在它上面
# 还叠了三层 —— Provider 的 NDJSON 协议、进度持久化(.d2x/state.json)、文件
# 监听循环。任何一层断掉,`mcpp test` 都照样全绿而 checker 已经废了;Windows
# 上就真出过整条链路静默失效、CI 却是绿的回归(见 d2x#35 / d2mcpp#87)。
#
# 判据(缺一不可):
#   1) 退出码 0                   —— checker 主动跑完退出
#   2) 渲染出的 passed 条目数 >= 参考答案数
#      —— 防空转:陈旧的 .d2x/state.json 会让 checker 一道题都不编译就直接
#         宣告完成并退 0。脚本会先删掉状态,这条是第二道保险。
#
# 用法:  bash d2x/buildtools/tests/checker-e2e.sh [zh|en]   (默认 zh)
# 环境:  D2X=<d2x 可执行文件路径>  (默认取 PATH 里的 d2x)
set -uo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
cd "$ROOT"

LANG_SEL="${1:-zh}"
D2X="${D2X:-d2x}"
LIMIT="${CHECKER_TIMEOUT:-1800}"     # 冷机第一次要下工具链 + 编译 52 道题

case "$LANG_SEL" in
    zh) PREFIX="src/" ;;
    en) PREFIX="src/en/" ;;
    *)  echo "用法: $0 [zh|en]"; exit 2 ;;
esac

MEMBERS=("${PREFIX}intro" "${PREFIX}cpp11" "${PREFIX}cpp14")
TEST_DIRS=()
for m in "${MEMBERS[@]}"; do TEST_DIRS+=("$m/tests"); done

# —— 脏树检查 —— 与 e2e.sh 同理:脚本要把答案覆盖到练习上再用 git 还原,
# 未提交的改动会被直接覆盖丢失。
dirty=$(git status --porcelain -- "${TEST_DIRS[@]}" solutions/ .d2x.json | head -20)
if [[ -n "$dirty" ]]; then
    echo "CHECKER-E2E($LANG_SEL) 拒绝运行: 练习/答案目录存在未提交改动(覆盖-还原流程会将其丢失):"
    echo "$dirty"
    exit 1
fi

restore() {
    git checkout -q -- "${TEST_DIRS[@]}" .d2x.json 2>/dev/null
    rm -rf .d2x/state.json .d2x/result.ndjson .d2x/last-output.log .d2x/checker.lock
}
trap restore EXIT

# —— 覆盖参考答案(zh/en 共用同一份 solutions)——
n_expected=0
while IFS= read -r sol; do
    rel="${sol#solutions/tests/}"    # intro/hello-mcpp.cpp | cpp11/00-x/0.cpp
    std="${rel%%/*}"; rest="${rel#*/}"
    dst="${PREFIX}${std}/tests/${rest}"
    if [[ ! -f "$dst" ]]; then
        echo "CHECKER-E2E($LANG_SEL) FAIL: 答案 $sol 没有对应的练习文件 $dst"
        exit 1
    fi
    cp "$sol" "$dst"
    n_expected=$((n_expected + 1))
done < <(find solutions/tests -name '*.cpp' | sort)

if [[ "$n_expected" -eq 0 ]]; then
    echo "CHECKER-E2E($LANG_SEL) FAIL: 一份参考答案都没找到"
    exit 1
fi

# Provider 按 .d2x.json 的 "lang" 决定扫 src/ 还是 src/en/(d2x --lang 只切界面
# 语言,不切课程目录),所以这里改的是配置而不是命令行。
if [[ "$LANG_SEL" == "en" ]]; then
    sed -i.bak 's/"lang": *"zh"/"lang": "en"/' .d2x.json && rm -f .d2x.json.bak
fi

# 必须从零开始:陈旧的进度会让 checker 一道题都不跑就宣告完成。
rm -rf .d2x/state.json .d2x/result.ndjson .d2x/last-output.log .d2x/checker.lock

OUT="$ROOT/.d2x/checker-e2e.out"
mkdir -p "$ROOT/.d2x"

# 看门狗用 shell 自己实现,不用 timeout(1):macOS 默认没有这个命令。
#
# 逐秒轮询而不是 `sleep $LIMIT`:后者即使被 kill 掉父 subshell,sleep 自己
# 也会作为孤儿活到时限,一直攥着继承来的 stdout —— 调用方一旦把本脚本接进
# 管道(CI 里常见),管道就要等满整个 LIMIT 才关,本来几分钟的任务看起来
# 像挂了半小时。轮询版在 d2x 退出后 1 秒内自己收摊。
watchdog() {
    local target="$1" limit="$2" waited=0
    while kill -0 "$target" 2>/dev/null; do
        if [[ "$waited" -ge "$limit" ]]; then
            kill -9 "$target" 2>/dev/null
            return
        fi
        sleep 1; waited=$((waited + 1))
    done
}

"$D2X" checker --ui print --lang en > "$OUT" 2>&1 &
d2x_pid=$!
watchdog "$d2x_pid" "$LIMIT" >/dev/null 2>&1 &
watchdog_pid=$!
wait "$d2x_pid"; code=$?
kill "$watchdog_pid" 2>/dev/null; wait "$watchdog_pid" 2>/dev/null

echo "------------------ checker output (tail) ------------------"
tail -n 30 "$OUT" 2>/dev/null
echo "-----------------------------------------------------------"

# 判据 1:退出码
if [[ "$code" != 0 ]]; then
    echo "CHECKER-E2E($LANG_SEL) FAIL: checker 退出码 $code —— 答案已全部覆盖,它本应走完全部练习后退 0"
    echo "(退出码 137 = 被看门狗按 ${LIMIT}s 上限杀掉,通常意味着卡在某道题上不推进)"
    exit 1
fi

# 判据 2:防空转
n_passed=$(grep -c "Status:.*passed" "$OUT" 2>/dev/null)
n_passed="${n_passed:-0}"
if [[ "$n_passed" -lt "$n_expected" ]]; then
    echo "CHECKER-E2E($LANG_SEL) FAIL: 防空转 —— 覆盖了 $n_expected 份答案,但 checker 只渲染出 $n_passed 条通过记录"
    echo "(checker 很可能读到了陈旧进度直接退出,而没有真的编译练习)"
    exit 1
fi

echo "CHECKER-E2E($LANG_SEL): checker 自主走完 $n_passed/$n_expected 道练习并正常退出 ✓"
