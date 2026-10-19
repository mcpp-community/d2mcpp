#!/usr/bin/env bash
# Source this file to retain PATH in the calling shell.
set -euo pipefail
INSTALL_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$INSTALL_ROOT/scripts/tool-versions.env"
export XLINGS_NON_INTERACTIVE=1
export PATH="$HOME/.xlings/subos/current/bin:$HOME/.xlings/bin:$PATH"
if ! command -v xlings >/dev/null 2>&1 || [[ "$(xlings --version)" != "xlings $XLINGS_VERSION" ]]; then
    installer="$(mktemp)"
    curl --retry 3 -fsSL "https://raw.githubusercontent.com/openxlings/xlings/v${XLINGS_VERSION}/tools/other/quick_install.sh" -o "$installer"
    bash "$installer" "v$XLINGS_VERSION"
    rm -f "$installer"
fi
[[ "$(xlings --version)" == "xlings $XLINGS_VERSION" ]] || { echo "Unexpected xlings version" >&2; return 1 2>/dev/null || exit 1; }
if [[ -n "${GITHUB_PATH:-}" ]]; then
    echo "$HOME/.xlings/subos/current/bin" >> "$GITHUB_PATH"
fi
xlings --version
