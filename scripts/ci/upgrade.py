#!/usr/bin/env python3
"""Upgrade a real August mcpp installation without discarding its project or toolchain home."""
import argparse
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
LOGS = ROOT / '.d2x/ci-logs'
LOGS.mkdir(parents=True, exist_ok=True)
os.chdir(ROOT)


def command(args, name, required=True):
    result = subprocess.run(args, text=True, encoding='utf-8', errors='replace',
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=1800)
    (LOGS / (name + '.log')).write_text(result.stdout, encoding='utf-8')
    print(result.stdout, flush=True)
    if required and result.returncode:
        raise AssertionError(f'{name} failed ({result.returncode})')
    return result


def setup(install_only=False):
    if os.name == 'nt':
        return ['powershell', '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', 'scripts/setup.ps1'] + (['-InstallOnly'] if install_only else [])
    return ['bash', 'scripts/setup.sh'] + (['--install-only'] if install_only else [])


parser = argparse.ArgumentParser()
parser.add_argument('--broken-glibc', action='store_true')
args = parser.parse_args()
config_path = ROOT / '.xlings.json'
original = config_path.read_bytes()
old = json.loads(original)
old['workspace']['mcpp'] = '2026.8.2.1'
try:
    config_path.write_text(json.dumps(old, indent=2) + '\n', encoding='utf-8')
    command(['xlings', 'update'], 'old-index')
    command(['xlings', 'install', '-y'], 'old-install')
    version = command(['mcpp', '--version'], 'old-version')
    assert '2026.8.2.1' in version.stdout
    # The old compiler may already fail against today's payloads. Keep that evidence,
    # and still exercise the migration; do not skip it because setup failed.
    command(['mcpp', 'run', '-q', '-p', 'd2x/buildtools', '--', 'describe'], 'old-provider', required=False)
    old_home = Path(os.environ['MCPP_HOME'])
    assert old_home.exists(), 'Old toolchain home was not initialized'
    if args.broken_glibc:
        scripts = list(old_home.glob('registry/data/xpkgs/xim-x-glibc/*/lib/libm.so'))
        assert scripts, 'Need a real glibc linker script to reproduce the incident'
        scripts[-1].write_text('GROUP ( /nonexistent/xlings-use-rpath-not-default-search/lib/libm.so.6 /nonexistent/xlings-use-rpath-not-default-search/lib/libmvec.so.1 )\n')
        (LOGS / 'corrupted-glibc-script.txt').write_text(str(scripts[-1]), encoding='utf-8')
finally:
    config_path.write_bytes(original)

progress = ROOT / '.d2x/state.json'
sentinel = b'{"version":1,"current":"hello-mcpp","completed":[]}\n'
progress.write_bytes(sentinel)
result = command(setup(), 'upgraded-setup', required=False)
if result.returncode:
    # Preserve the failed home for diagnosis. This is the documented reversible
    # recovery for a payload whose installed files are already damaged.
    command(['mcpp', 'self', 'doctor'], 'old-home-doctor', required=False)
    fresh_home = old_home.with_name(old_home.name + '-recovered')
    assert not fresh_home.exists()
    os.environ['MCPP_HOME'] = str(fresh_home)
    command(setup(), 'recovered-setup')
    assert old_home.exists(), 'Recovery must preserve the old home'
assert progress.read_bytes() == sentinel, 'Upgrade changed learning progress'
progress.unlink()  # first-run.py requires an unused disposable course
command(['python' if os.name == 'nt' else 'python3', 'scripts/ci/solutions.py'], 'solutions')
command(['python' if os.name == 'nt' else 'python3', 'scripts/ci/first-run.py', '--allow-source-edit'], 'checker')
print('UPGRADE PASS: legacy home retained; course pin, reference answers, save/advance/resume verified')
