#!/usr/bin/env bash
# Fail creation if the environment cannot actually build and run the course.
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."
rm -f .d2x/environment-ready
sudo apt-get update
sudo apt-get install -y ncurses-bin libtinfo6 libncursesw6 curl ca-certificates git
export XLINGS_NON_INTERACTIVE=1
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
bash scripts/setup.sh
mcpp test -p solutions
mkdir -p .d2x
bash .devcontainer/readiness.sh fingerprint > .d2x/environment-ready
echo 'd2mcpp environment ready. In a terminal run: d2x checker'
