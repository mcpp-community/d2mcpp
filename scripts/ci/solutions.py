#!/usr/bin/env python3
"""Run the documented environment check, and reject an empty/partial green run."""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LOGS = ROOT / '.d2x/ci-logs'
sys.stdout.reconfigure(encoding='utf-8', errors='replace')
sys.stderr.reconfigure(encoding='utf-8', errors='replace')

LOGS.mkdir(parents=True, exist_ok=True)
result = subprocess.run(['mcpp', 'test', '-p', 'solutions'], cwd=ROOT, text=True,
                        encoding='utf-8', errors='replace', stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                        timeout=1800)
(LOGS / 'solutions.log').write_text(result.stdout, encoding='utf-8')
print(result.stdout, flush=True)
expected = len(list((ROOT / 'solutions/tests').rglob('*.cpp')))
summaries = re.findall(r'(\d+) passed; (\d+) failed', result.stdout)
assert expected > 0 and result.returncode == 0 and summaries, 'Reference solution build failed or no summary'
assert summaries[-1] == (str(expected), '0'), (expected, summaries[-1])
print(f'SOLUTIONS PASS: {expected}/{expected}')
