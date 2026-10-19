#!/usr/bin/env python3
"""Upgrade a real August mcpp installation without discarding its project or toolchain home."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import io
import shutil
import tarfile
import urllib.request

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
parser.add_argument('--variant', choices=['existing', 'broken-glibc', 'stale-index', 'dangling-record'], default='existing')
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
    if args.variant == 'broken-glibc':
        scripts = list(old_home.glob('registry/data/xpkgs/xim-x-glibc/*/lib/libm.so'))
        assert scripts, 'Need a real glibc linker script to reproduce the incident'
        scripts[-1].write_text('GROUP ( /nonexistent/xlings-use-rpath-not-default-search/lib/libm.so.6 /nonexistent/xlings-use-rpath-not-default-search/lib/libmvec.so.1 )\n')
        (LOGS / 'corrupted-glibc-script.txt').write_text(str(scripts[-1]), encoding='utf-8')
    if args.variant == 'stale-index':
        # Historical package recipes, with the old home's payloads kept intact.
        snapshot = '240c2f78fce8a1bce65a59c0e8e40c9fff2a4eab'
        index = old_home / 'registry/data/xim-pkgindex'
        assert index.is_dir()
        archive = urllib.request.urlopen(f'https://codeload.github.com/openxlings/xim-pkgindex/tar.gz/{snapshot}', timeout=120).read()
        with tarfile.open(fileobj=io.BytesIO(archive)) as tar:
            temporary = LOGS / 'index-snapshot'
            temporary.mkdir()
            # This fixture is Linux-only and uses the hosted runner's Python 3.12+.
            tar.extractall(temporary, filter='data')
        source = next(temporary.iterdir())
        # Preserve the provider's index version stamp, as the upstream upgrade
        # lane does, while replacing the recipes a client actually reads.
        for child in index.iterdir():
            if child.name == '.xlings-index-version':
                continue
            shutil.rmtree(child) if child.is_dir() else child.unlink()
        shutil.copytree(source, index, dirs_exist_ok=True)
        shutil.rmtree(temporary)
        (LOGS / 'stale-index.txt').write_text(snapshot, encoding='utf-8')
    if args.variant == 'dangling-record':
        registry = old_home / 'registry'
        db_paths = [registry / 'data/versions.json', registry / '.xlings.json']
        db = next(p for p in db_paths if p.exists())
        records = json.loads(db.read_text())['versions'].get('c++', {}).get('versions', {})
        key = next(k for k, v in records.items() if 'gcc' in v.get('bindingGroup', {}).get('provider', ''))
        active = registry / 'subos/default/.xlings.json'
        doc = json.loads(active.read_text())
        entry = doc['workspace'].get('c++')
        if isinstance(entry, dict):
            entry['active'] = key
        else:
            doc['workspace']['c++'] = key
        active.write_text(json.dumps(doc), encoding='utf-8')
        for path in db_paths:
            if path.exists():
                doc = json.loads(path.read_text())
                cpp = doc.get('versions', {}).get('c++', {})
                cpp.get('versions', {}).pop(key, None)
                for edges in cpp.get('bindings', {}).values():
                    edges.pop(key, None)
                path.write_text(json.dumps(doc), encoding='utf-8')
        (LOGS / 'dangling-record.txt').write_text(key, encoding='utf-8')
finally:
    config_path.write_bytes(original)

progress = ROOT / '.d2x/state.json'
sentinel = b'{"version":1,"current":"hello-mcpp","completed":["cpp11-00-auto-and-decltype-0"]}\n'
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
if args.variant == 'dangling-record':
    command(['mcpp', 'toolchain', 'install', 'gcc', '16.1.0'], 'provision-after-dangling-record')
progress.unlink()  # first-run.py requires an unused disposable course
command(['python' if os.name == 'nt' else 'python3', 'scripts/ci/solutions.py'], 'solutions')
command(['python' if os.name == 'nt' else 'python3', 'scripts/ci/first-run.py', '--allow-source-edit'], 'checker')
print('UPGRADE PASS: legacy home retained; course pin, reference answers, save/advance/resume verified')
