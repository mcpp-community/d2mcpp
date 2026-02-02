@echo off
REM ===== 配置参数 =====
set "UPSTREAM_URL=https://github.com/mcpp-community/d2mcpp.git"
set "BRANCH_NAME=main"
set "HAS_CHANGES="
set "HAS_COMMITS="
set "BACKUP_REQUIRED="
set "TIMESTAMP="
set "BACKUP_BRANCH="
set "COMMITS_AHEAD="

echo [1] - 检查是否已添加 upstream 远程...
git remote | findstr /b /c:"upstream" >nul
if errorlevel 1 (
    echo ➕ 添加 upstream: %UPSTREAM_URL%
    git remote add upstream %UPSTREAM_URL%
) else (
    echo ✅ upstream 已存在
)

echo [2] - 获取 upstream 更新...
git fetch upstream
if errorlevel 1 goto error

echo [3] - 切换到 %BRANCH_NAME% 分支...
git checkout %BRANCH_NAME%
if errorlevel 1 goto error

echo [4] - 检测到本地更改或提交，正在创建备份...

REM 注: 不能放到()内，() 代码块中使用 %变量%, 运行时不会更新
for /f %%i in ('powershell -Command "Get-Date -Format yyyyMMdd-HHmmss"') do set TIMESTAMP=%%i
set BACKUP_BRANCH=solutions-%TIMESTAMP%

echo 🔀 创建备份分支: %BACKUP_BRANCH%
git checkout -b %BACKUP_BRANCH%
if errorlevel 1 goto error

REM ===== 检查是否有未提交更改 =====
git diff --quiet
if errorlevel 1 set HAS_CHANGES=1

git diff --cached --quiet
if errorlevel 1 set HAS_CHANGES=1

if defined HAS_CHANGES (
    echo 📦 提交本地未提交更改...
    git add .
    git commit -m "🧶 自动备份：本地改动"
    if errorlevel 1 goto error
)

echo ✅ 本地改动已保存到 %BACKUP_BRANCH%

echo 🔁 回到 %BRANCH_NAME% 分支...
git checkout %BRANCH_NAME%

echo [5] - 使用 rebase 同步 upstream/%BRANCH_NAME% 到本地 %BRANCH_NAME%...
git rebase upstream/%BRANCH_NAME%
if errorlevel 1 (
    goto error
)

echo ✅ 更新完成！
goto end

:error
echo ❌ 出现冲突，请手动解决后完成 rebase
exit /b 1

:end