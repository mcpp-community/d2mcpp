# 环境准备与排错

[English](environment.md) · [快速开始](../README.zh.md)

支持 Linux x86_64、macOS 14+ Apple Silicon、Windows x86_64 / PowerShell 5.1+。工具版本由 [`.xlings.json`](../.xlings.json) 管理。

<a id="install-xlings"></a>

## 安装 xlings

Linux / macOS：

```bash
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/v2026.10.18.1/tools/other/quick_install.sh | bash -s v2026.10.18.1
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
```

Windows PowerShell：

```powershell
irm https://raw.githubusercontent.com/mcpp-community/d2mcpp/main/scripts/install-xlings.ps1 | iex
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
```

安装后继续 README 的步骤。找不到命令时，重新打开终端，或执行上面对应的 PATH 命令。

## Git 获取与更新

安装 xlings 后，也可克隆课程；使用 fork 时替换为自己的仓库地址：

```bash
git clone https://github.com/mcpp-community/d2mcpp.git
cd d2mcpp
```

<a id="prepare"></a>

在课程根目录准备环境；更新时先执行 `git pull`。脚本会保留练习代码和学习进度。

```bash
# Linux / macOS
bash scripts/setup.sh
```

```powershell
# Windows PowerShell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\setup.ps1
```

然后验证并开始练习：

```bash
mcpp test -p solutions
d2x checker
```

## Codespaces

等待准备完成，在终端运行 `d2x checker`。创建失败时查看创建日志；需要重试时，在容器内执行：

```bash
bash .devcontainer/postCreate.sh
```

## Provider 无响应

在课程根目录运行以下命令，查看具体错误，再重试[准备步骤](#prepare)：

```bash
xlings --version
mcpp --version
d2x --version
mcpp run -p d2x/buildtools -- describe
```

## `/nonexistent/.../libm.so.6` / `libmvec.so.1`

先更新课程并重新准备。若仍报错，选择未使用的工具链目录，再执行[准备与验证步骤](#prepare)：

```bash
# Linux / macOS
export MCPP_HOME="$HOME/.mcpp-d2mcpp-recovered"
```

```powershell
# Windows PowerShell
$env:MCPP_HOME = "$env:USERPROFILE\.mcpp-d2mcpp-recovered"
```

这会重新下载工具链，保留旧目录和学习进度。后续练习需设置同一个 `MCPP_HOME`；取消覆盖用 `unset MCPP_HOME`（Unix）或 `Remove-Item Env:MCPP_HOME`（PowerShell）。

仍有问题时，附系统、工具版本、失败命令和原始错误[提交 issue](https://github.com/mcpp-community/d2mcpp/issues)，不要上传 API key。

## 首次启动准备与版本核对

`d2x install` 会安装下载课程所声明的项目依赖。通过 Git 更新后，重新运行准备脚本以应用版本变化。
在课程目录执行 `d2x --version`，与 `.xlings.json` 对照；仅更新全局工具不能证明项目实际使用的版本。

checker 在第一题之前显示准备信息。首次 Provider 启动的总期限为 900 秒，后续调用的空闲期限为 120 秒；
准备提示不会延长期限。失败时保留 `.d2x/provider.stdout.log`、`.d2x/provider.stderr.log` 与命令输出。
启动和传输错误会非零退出；未完成练习的 fail 判定属于预期行为。

已有工具环境先执行 `xlings update`，再执行 `xlings install --use d2x@latest mcpp@latest -y`。`@latest` 选择最新稳定条目；`--use` 即使新版已安装也会激活它。仅执行不带版本或激活选项的 install 可能继续保留旧版。

用户取消返回 130 和 `cancelled` 事件，不报告 Provider 故障，也不生成练习失败判定。
