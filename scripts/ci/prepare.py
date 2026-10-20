#!/usr/bin/env python3
"""Run native course preparation with live output and archived raw streams."""
import os
from pathlib import Path
import subprocess
import sys
import threading

ROOT = Path(__file__).resolve().parents[2]
LOGS = ROOT / '.d2x/ci-logs'
LOGS.mkdir(parents=True, exist_ok=True)
install_only = sys.argv[1:] == ['--install-only']
assert not sys.argv[1:] or install_only, 'Usage: prepare.py [--install-only]'
command = (['powershell', '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', 'scripts/setup.ps1']
           + (['-InstallOnly'] if install_only else [])) if os.name == 'nt' else (
           ['bash', 'scripts/setup.sh'] + (['--install-only'] if install_only else []))
process = subprocess.Popen(command, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.PIPE)


def copy(stream, destination, suffix):
    with (LOGS / f'prepare.{suffix}').open('wb') as log:
        for chunk in iter(stream.readline, b''):
            log.write(chunk)
            log.flush()
            destination.write(chunk)
            destination.flush()


threads = [threading.Thread(target=copy, args=(process.stdout, sys.stdout.buffer, 'stdout')),
           threading.Thread(target=copy, args=(process.stderr, sys.stderr.buffer, 'stderr'))]
for thread in threads:
    thread.start()
code = process.wait()
for thread in threads:
    thread.join()
sys.exit(code)
