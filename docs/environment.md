# Environment setup and troubleshooting

[中文](environment.zh.md) · [Quick start](../README.md)

Supports Linux x86_64, macOS 14+ Apple Silicon, and Windows x86_64 / PowerShell 5.1+. Tool versions are managed in [`.xlings.json`](../.xlings.json).

<a id="install-xlings"></a>

## Install xlings

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

Continue with the README. If a command is not found, reopen your terminal or apply the PATH command above.

## Git installation and updates

After installing xlings, you can also clone the course. Replace the URL with your fork if needed:

```bash
git clone https://github.com/mcpp-community/d2mcpp.git
cd d2mcpp
```

<a id="prepare"></a>

Prepare from the course root; run `git pull` first when updating. These scripts preserve your exercise code and progress.

```bash
# Linux / macOS
bash scripts/setup.sh
```

```powershell
# Windows PowerShell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\setup.ps1
```

Then verify and practice:

```bash
mcpp test -p solutions
d2x checker
```

## Codespaces

Wait until ready, then run `d2x checker` in a terminal. If creation fails, check the creation log. To retry inside the container:

```bash
bash .devcontainer/postCreate.sh
```

## Provider did not respond

Run from the course root to expose the error, then retry [preparation](#prepare):

```bash
xlings --version
mcpp --version
d2x --version
mcpp run -p d2x/buildtools -- describe
```

## `/nonexistent/.../libm.so.6` / `libmvec.so.1`

Update the course and rerun preparation. If the error persists, choose an unused toolchain directory and repeat [preparation and verification](#prepare):

```bash
# Linux / macOS
export MCPP_HOME="$HOME/.mcpp-d2mcpp-recovered"
```

```powershell
# Windows PowerShell
$env:MCPP_HOME = "$env:USERPROFILE\.mcpp-d2mcpp-recovered"
```

This downloads a new toolchain, keeping the old directory and progress. Set the same `MCPP_HOME` in future sessions. Undo with `unset MCPP_HOME` on Unix or `Remove-Item Env:MCPP_HOME` in PowerShell.

Still stuck? [Open an issue](https://github.com/mcpp-community/d2mcpp/issues) with your OS, tool versions, command and error. Keep API keys out of reports.

## First-start preparation and version checks

`d2x install` installs the downloaded course's declared workspace dependencies.
After a Git update, rerun the preparation script to apply changed versions.
Run `d2x --version` from the course directory and compare it with `.xlings.json`;
updating a global tool alone does not establish which project version is selected.

The checker shows preparation before the first exercise. Initial Provider startup
has a total 900-second budget; subsequent Provider calls have a 120-second idle
limit. Preparation messages do not extend the startup deadline. On failure, keep
`.d2x/provider.stdout.log`, `.d2x/provider.stderr.log` and the command output.
Startup/transport errors exit nonzero; an unfinished exercise verdict is expected.

For an existing tool installation, use `xlings install --use d2x@latest mcpp@latest -y` after `xlings update`. `@latest` selects the newest stable index entry; `--use` activates it even when already installed. Bare install commands can retain the previously selected version.

User cancellation returns 130 and a `cancelled` event without a Provider-failure warning or a failed exercise verdict.
