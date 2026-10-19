#!/usr/bin/env bash
# Prepare the course without changing exercises or learning progress.
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."
trap 'echo "Environment preparation failed. Keep the error above; see docs/environment.md." >&2' ERR
source scripts/install-xlings.sh
echo '[1/3] Installing the course versions from .xlings.json'
xlings update
xlings install -y
for tool in mcpp d2x; do
    expected=$(sed -n "s/.*\"$tool\": *\"\([^\"]*\)\".*/\1/p" .xlings.json)
    actual=$("$tool" --version)
    command -v "$tool"
    echo "$actual"
    [[ "${actual##* }" == "$expected" ]] || { echo "Expected $tool $expected" >&2; exit 1; }
done
if [[ "${1:-}" == '--install-only' ]]; then exit 0; fi
if [[ $# -ne 0 ]]; then echo "Usage: bash scripts/setup.sh [--install-only]" >&2; exit 2; fi
echo '[2/3] Building the Provider (the first build downloads its C++ toolchain)'
mcpp build -p d2x/buildtools
echo '[3/3] Checking the Provider'
description=$(mcpp run -q -p d2x/buildtools -- describe)
echo "$description"
grep -q '"event":"describe","protocol":1,"name":"mcpp"' <<< "$description"
exercises=$(mcpp run -q -p d2x/buildtools -- exercises)
grep -q '"event":"exercise"' <<< "$exercises"
echo 'Provider ready. Verify with: mcpp test -p solutions'
echo 'Then start practicing: d2x checker'
