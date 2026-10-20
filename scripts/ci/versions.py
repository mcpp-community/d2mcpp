#!/usr/bin/env python3
"""Record course pins, selected executables and an explicit CI candidate if supplied."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def checker_binary():
    return os.environ.get('D2X_CANDIDATE', 'd2x')


def verify(root=ROOT, label='versions'):
    pins = json.loads((root / '.xlings.json').read_text())['workspace']
    result = {'pins': pins, 'cwd': str(root), 'tools': {}}
    for name in pins:
        actual = subprocess.check_output([name, '--version'], cwd=root, text=True, encoding='utf-8').strip()
        path = shutil.which(name)
        result['tools'][name] = {'version': actual, 'path': path}
        assert actual.split()[-1].removeprefix('v') == pins[name], f'{name}: actual {actual}; expected {pins[name]}; path {path}'
    if os.environ.get('D2X_CANDIDATE'):
        binary = Path(os.environ['D2X_CANDIDATE'])
        provenance = json.loads(binary.with_name('provenance.json').read_text())
        assert provenance['repository'] == 'd2learn/d2x'
        assert provenance['sha'] == os.environ['D2X_CANDIDATE_SHA']
        assert hashlib.sha256(binary.read_bytes()).hexdigest() == provenance['sha256']
        actual = subprocess.check_output([str(binary), '--version'], text=True).strip()
        assert actual == provenance['version']
        result['candidate'] = {**provenance, 'path': str(binary)}
        # Explicit prerelease override, never presented as a stable installation.
    logs = root / '.d2x/ci-logs'
    logs.mkdir(parents=True, exist_ok=True)
    (logs / f'{label}.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2), flush=True)
    return result


if __name__ == '__main__':
    import sys
    if len(sys.argv) == 3 and sys.argv[1] == '--public':
        report = {tool: {'version': subprocess.check_output([tool, '--version'], text=True).strip(),
                         'path': shutil.which(tool)} for tool in ['d2x', 'mcpp', 'xlings']}
        Path(sys.argv[2]).write_text(json.dumps(report, indent=2))
        print('Public install versions:', json.dumps(report), flush=True)
    else:
        verify()
