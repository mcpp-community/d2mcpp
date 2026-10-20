#!/usr/bin/env python3
"""Verify public latest independently, then verify the reviewed project pins."""
import json
import os
from pathlib import Path
import subprocess
import tempfile
import urllib.request

from versions import verify

root = Path(__file__).resolve().parents[2]
logs = root / '.d2x/ci-logs'
logs.mkdir(parents=True, exist_ok=True)
headers = {'Accept': 'application/vnd.github+json'}
if os.environ.get('GH_TOKEN'):
    headers['Authorization'] = 'Bearer ' + os.environ['GH_TOKEN']
request = urllib.request.Request('https://api.github.com/repos/d2learn/d2x/releases/latest', headers=headers)
with urllib.request.urlopen(request, timeout=60) as response:
    release = json.load(response)
latest = release['tag_name'].removeprefix('v')
pin = json.loads((root / '.xlings.json').read_text())['workspace']['d2x']
with tempfile.TemporaryDirectory(prefix='d2x-public-latest-') as neutral:
    subprocess.run(['xlings', 'update'], cwd=neutral, check=True)
    subprocess.run(['xlings', 'install', '--use', 'd2x@2026.08.02.2', '-y'], cwd=neutral, check=True)
    old = subprocess.check_output(['d2x', '--version'], cwd=neutral, text=True).strip()
    assert old == '2026.08.02.2', 'Old-version activation fixture failed'
    subprocess.run(['xlings', 'install', '--use', 'd2x@latest', '-y'], cwd=neutral, check=True)
    actual = subprocess.check_output(['d2x', '--version'], cwd=neutral, text=True).strip()
report = {'previous': old, 'latest': latest, 'course_pin': pin, 'public_install': actual, 'release': release['html_url']}
(logs / 'latest.json').write_text(json.dumps(report, indent=2))
print(json.dumps(report, indent=2), flush=True)
assert actual == latest, 'Public index/install is behind the stable release'
# A new upstream release must not force an unreviewed course dependency bump.
manifest = (root / '.xlings.json').read_bytes()
subprocess.run(['xlings', 'install', '-y'], cwd=root, check=True)
verify(label='pinned-after-latest')
assert (root / '.xlings.json').read_bytes() == manifest, 'Install changed the reviewed manifest'
