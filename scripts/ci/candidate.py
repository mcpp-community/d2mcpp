#!/usr/bin/env python3
"""Activate only an explicitly requested, checksum-verified d2x CI artifact."""
import hashlib
import json
import os
from pathlib import Path
import subprocess

root = Path(os.environ['D2X_CANDIDATE_DIR']).resolve()
metadata = json.loads((root / 'provenance.json').read_text())
assert metadata['repository'] == 'd2learn/d2x'
assert metadata['sha'] == os.environ['D2X_CANDIDATE_SHA']
assert metadata['file'] in ['d2x', 'd2x.exe']
binary = root / metadata['file']
assert hashlib.sha256(binary.read_bytes()).hexdigest() == metadata['sha256']
binary.chmod(0o755)
assert subprocess.check_output([str(binary), '--version'], text=True).strip() == metadata['version']
with open(os.environ['GITHUB_ENV'], 'a', encoding='utf-8') as env:
    env.write(f'D2X_CANDIDATE={binary}\n')
print('Explicit d2x candidate:', json.dumps(metadata), flush=True)
