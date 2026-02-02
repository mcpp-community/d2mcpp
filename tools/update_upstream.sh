#!/bin/bash

# 脚本名称：update_upstream.sh

set -euo pipefail

UPSTREAM_URL="https://github.com/mcpp-community/d2mcpp.git"
BRANCH_NAME="main"

echo "[1] - 检查是否已添加 upstream 远程..."
if ! git remote get-url upstream &> /dev/null; then
    echo "添加 upstream: $UPSTREAM_URL"
    git remote add upstream "$UPSTREAM_URL"
else
    echo "✅ upstream 已存在"
fi

echo "[2] - 获取 upstream 更新..."
git fetch upstream

echo "[3] - 切换到 $BRANCH_NAME 分支..."
git checkout $BRANCH_NAME


echo "[4] - 检测到本地改动，自动备份中..."

TIMESTAMP=$(date +"%Y%m%d-%H%M%S")
BACKUP_BRANCH="solutions-$TIMESTAMP"

echo "🔀 创建备份分支: $BACKUP_BRANCH"
git checkout -b "$BACKUP_BRANCH"

# 判断是否有工作区改动
HAS_UNSTAGED_CHANGES=false
if ! git diff --quiet || ! git diff --cached --quiet; then
    HAS_UNSTAGED_CHANGES=true
fi

if $HAS_UNSTAGED_CHANGES; then
    echo "📦 提交工作目录的未提交改动"
    git add .
    git commit -m "🧶 自动备份：本地改动 - $TIMESTAMP"
fi

echo "✅ 本地改动已保存到 $BACKUP_BRANCH 分支"

# 回到 main 分支准备 rebase
git checkout $BRANCH_NAME

echo "[5] - 使用 rebase 同步 upstream/$BRANCH_NAME 到本地 $BRANCH_NAME..."
if ! git rebase upstream/$BRANCH_NAME; then
    echo "❌ 出现冲突，请手动解决后完成 rebase"
    exit 1
fi

echo "✅ 更新完成！"