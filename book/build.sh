#!/bin/bash

# srcirpt dir
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

echo "[Top] - Building Chinese book..."
cd "$SCRIPT_DIR"
mdbook build

echo "[Sub - 1] - Building English book..."
cd "$SCRIPT_DIR"
cd en && mdbook build

echo "[imgs] - copy imgs to output directory..."
cd "$SCRIPT_DIR"
cp -r imgs book/

echo "Build completed."

python -m http.server --directory book/book
