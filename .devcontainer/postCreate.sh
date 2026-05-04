# postCreate.sh
#!/usr/bin/env bash
set -euo pipefail

sudo apt-get update
sudo apt-get install -y ncurses-bin libtinfo6 libncursesw6 curl ca-certificates git

if ! command -v xlings >/dev/null 2>&1; then
  curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash -s -- v0.4.14
fi

echo "xlings installed"

# source PATH so xlings/d2x are available in this shell
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
[ -f "$HOME/.bashrc" ] && source "$HOME/.bashrc"

xlings install
