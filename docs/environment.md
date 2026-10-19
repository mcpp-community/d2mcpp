# 环境准备与排错 / Environment setup and troubleshooting

首次使用请按 [README](../README.zh.md) 操作。工具版本由 [`.xlings.json`](../.xlings.json) 和[安装器配置](../scripts/tool-versions.env)管理。

For first use, follow the [README](../README.md). Tool versions are managed by the project configuration.

| 系统 / OS | 支持平台 / Supported platform |
| --- | --- |
| Linux | x86_64 |
| macOS | 14+，Apple Silicon |
| Windows | x86_64，PowerShell 5.1+ |

<a id="install-xlings"></a>

## 安装 xlings / Install xlings

Linux / macOS:

```bash
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/v2026.10.18.1/tools/other/quick_install.sh | bash -s v2026.10.18.1
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
```

Windows PowerShell:

```powershell
irm https://raw.githubusercontent.com/mcpp-community/d2mcpp/main/scripts/install-xlings.ps1 | iex
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
```

安装后继续 README 的步骤。若提示找不到命令，重新打开终端，或执行上面对应的 PATH 命令。

Continue with the README. If a command is not found, reopen your terminal or apply the PATH command above.

## Git 获取与更新 / Git installation and updates

安装 xlings 后，也可通过 Git 获取课程。若先 fork，将下面地址替换为自己的仓库：

After installing xlings, you can also get the course with Git. To use a fork, replace the URL below with your repository:

```bash
git clone https://github.com/mcpp-community/d2mcpp.git
cd d2mcpp
```

<a id="prepare"></a>

在课程根目录准备环境；已有 Git 仓库更新时，先执行 `git pull`：

Prepare from the course root. For an existing Git checkout, run `git pull` first:

```bash
# Linux / macOS
bash scripts/setup.sh
```

```powershell
# Windows PowerShell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\setup.ps1
```

然后验证并开始练习。准备脚本可重复运行，会保留练习代码和学习进度。

Then verify and practice. Preparation is repeatable and preserves your code and progress.

```bash
mcpp test -p solutions
d2x checker
```

## Codespaces

等待准备完成，在终端运行 `d2x checker`，或选择 **Tasks: Run Task → d2mcpp: practice**。创建失败时查看创建日志；需要重新准备时，在容器内执行：

Wait until ready, then run `d2x checker` or the **d2mcpp: practice** task. If creation fails, check the creation log. To rerun preparation inside the container:

```bash
bash .devcontainer/postCreate.sh
```

## Provider 无响应 / Provider did not respond

在课程根目录运行以下命令，查看具体错误；需要重新准备时使用[上面的脚本](#prepare)。

Run these commands from the course root to expose the underlying error. Use the [preparation scripts above](#prepare) to retry setup.

```bash
xlings --version
mcpp --version
d2x --version
mcpp run -p d2x/buildtools -- describe
```

## `/nonexistent/.../libm.so.6` / `libmvec.so.1`

先更新课程并重新准备。若仍出现此错误，选择一个未使用的目录作为新工具链目录，再执行[准备与验证步骤](#prepare)：

Update the course and rerun preparation. If the error persists, choose an unused toolchain directory and repeat the [preparation and verification steps](#prepare):

```bash
# Linux / macOS
export MCPP_HOME="$HOME/.mcpp-d2mcpp-recovered"
```

```powershell
# Windows PowerShell
$env:MCPP_HOME = "$env:USERPROFILE\.mcpp-d2mcpp-recovered"
```

这会重新下载工具链，保留旧目录和学习进度。后续练习需设置同一个 `MCPP_HOME`；取消覆盖用 `unset MCPP_HOME`（Unix）或 `Remove-Item Env:MCPP_HOME`（PowerShell）。

This downloads a new toolchain while keeping the old directory and progress. Set the same `MCPP_HOME` in future sessions; undo with `unset MCPP_HOME` on Unix or `Remove-Item Env:MCPP_HOME` in PowerShell.

仍有问题时，附系统、工具版本、失败命令和原始错误[提交 issue](https://github.com/mcpp-community/d2mcpp/issues)。新版 mcpp 可用 `mcpp self doctor` 收集诊断；不要上传含 API key 的配置。

If the problem remains, [open an issue](https://github.com/mcpp-community/d2mcpp/issues) with your OS, tool versions, command and error. Use `mcpp self doctor` for diagnostics on newer mcpp versions. Keep API keys out of reports.
