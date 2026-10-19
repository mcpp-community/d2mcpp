#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."
fingerprint() {
    sha256sum .xlings.json .d2x.json scripts/tool-versions.env scripts/setup.sh \
        scripts/install-xlings.sh .devcontainer/postCreate.sh .devcontainer/devcontainer.json
}
if [[ "${1:-}" == fingerprint ]]; then fingerprint; exit 0; fi
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
if [[ ! -f .d2x/environment-ready ]] || [[ "$(cat .d2x/environment-ready)" != "$(fingerprint)" ]]; then
    echo 'The environment needs preparation. Run: bash .devcontainer/postCreate.sh' >&2
    exit 1
fi
source scripts/tool-versions.env
[[ "$(xlings --version)" == "xlings $XLINGS_VERSION" ]]
for tool in mcpp d2x; do
    expected=$(sed -n "s/.*\"$tool\": *\"\([^\"]*\)\".*/\1/p" .xlings.json)
    actual=$("$tool" --version)
    [[ "${actual##* }" == "$expected" ]]
done
echo 'd2mcpp is ready. Start in the terminal: d2x checker'
echo 'Or run the VS Code task: d2mcpp: practice'
