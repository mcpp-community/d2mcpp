#!/usr/bin/env python3
"""Compare baseline build and checker using independent CI jobs, never a shared cache."""
import json
import os
from pathlib import Path
import subprocess
import sys
import time

from versions import verify

logs = Path('.d2x/ci-logs')
logs.mkdir(parents=True, exist_ok=True)
verify(label='diagnostic-baseline-versions')
assert not Path('target').exists()
assert not Path(os.environ['MCPP_HOME']).exists()
route = os.environ['DIAGNOSTIC_ROUTE']
command = ['mcpp', 'build', '-p', 'd2x/buildtools'] if route == 'build' else [sys.executable, 'scripts/ci/first-run.py', '--allow-source-edit', '--cold']
started = time.monotonic()
with (logs / f'{route}.log').open('w', encoding='utf-8') as output:
    # The baseline checker may hit its known 120s error. Keep its actual result.
    result = subprocess.run(command, stdout=output, stderr=subprocess.STDOUT, timeout=1500)
report = {'route': route, 'exit': result.returncode, 'elapsed': time.monotonic() - started}
(logs / 'diagnostic-result.json').write_text(json.dumps(report, indent=2))
for source in Path('.d2x').glob('provider.*.log'):
    (logs / source.name).write_bytes(source.read_bytes())
print(json.dumps(report), flush=True)
print((logs / f'{route}.log').read_text(errors='replace')[-16000:], flush=True)
# Build must complete. Baseline checker failure is the evidence under investigation.
if route == 'build':
    assert result.returncode == 0
