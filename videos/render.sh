#!/usr/bin/env bash
# d2mcpp 动画渲染启动器 —— 封装 PKG_CONFIG_PATH(项目 subos)+ uv 依赖 pin,免手输环境变量。
#
# 用法:
#   videos/render.sh <场景>            # 默认 -ql 快预览
#   videos/render.sh <场景> -qh        # 高清 1080p
#   videos/render.sh <场景> -qh -p     # 高清 + 渲染完自动打开
# <场景> 可写相对 videos/ 的路径或完整路径,例:
#   videos/render.sh cpp11/00-auto-and-decltype.py
#   videos/render.sh videos/cpp11/09-list-initialization.py -qh
#
# 前置(一次性,把 xcb-free 图形栈 + uv 装进项目 subos):
#   xlings install cairo pango uv
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

if [[ $# -lt 1 ]]; then
    echo "用法: videos/render.sh <场景.py> [manim 参数, 默认 -ql]" >&2
    exit 2
fi
SCENE="$1"; shift
[[ -f "$SCENE" ]] || SCENE="videos/$SCENE"          # 允许省略开头的 videos/
[[ -f "$SCENE" ]] || { echo "找不到场景文件: $1" >&2; exit 1; }

# manim 参数:默认 -ql(快预览),用户传了就用用户的(如 -qh / -pqh)
MANIM_ARGS=("$@"); [[ ${#MANIM_ARGS[@]} -gt 0 ]] || MANIM_ARGS=(-ql)

# 项目匿名 subos sysroot —— xlings install cairo pango 装在这, xlings-gcc 默认也搜它
SR="$REPO_ROOT/.xlings/subos/_/usr"
if [[ ! -f "$SR/lib/pkgconfig/cairo.pc" ]]; then
    echo "[render] 警告: 项目 subos 里没找到 cairo —— 先跑: xlings install cairo pango uv" >&2
    echo "[render]        (没装生态栈时也可在命令前加 CC=/usr/bin/gcc CXX=/usr/bin/g++ 兜底)" >&2
fi
export PKG_CONFIG_PATH="$SR/lib/pkgconfig${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}"
export LD_LIBRARY_PATH="$SR/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

echo "[render] scene = $SCENE   args = ${MANIM_ARGS[*]}"
# manim 本体 + pygments(pin 2.17.2 避开 0.18.1 的 Code 渲染 #CCC bug)由 uv 临时装
exec uv run --with "manim==0.18.1" --with "pygments==2.17.2" manim "${MANIM_ARGS[@]}" "$SCENE"
