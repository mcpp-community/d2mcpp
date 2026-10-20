#!/usr/bin/env python3
"""Compare the stable d2x release, course pin and real public latest install."""
import json
import os
from pathlib import Path
import subprocess
import tempfile
import urllib.request

root = Path(__file__).resolve().parents[2]
logs = root / '.d2x/ci-logs'
logs.mkdir(parents=True, exist_ok=True)
request = urllib.request.Request('https://api.github.com/repos/d2learn/d2x/releases/latest', headers={'Accept': 'application/vnd.github+json'})
with urllib.request.urlopen(request, timeout=60) as response:
    release = json.load(response)
latest = release['tag_name'].removeprefix('v')
pin = json.loads((root / '.xlings.json').read_text())['workspace']['d2x']
with tempfile.TemporaryDirectory(prefix='d2x-public-latest-') as neutral:
    subprocess.run(['xlings', 'update'], cwd=neutral, check=True)
    subprocess.run(['xlings', 'install', 'd2x', '-y'], cwd=neutral, check=True)
    actual = subprocess.check_output(['d2x', '--version'], cwd=neutral, text=True).strip()
report = {'latest': latest, 'course_pin': pin, 'public_install': actual, 'release': release['html_url']}
(logs / 'latest.json').write_text(json.dumps(report, indent=2))
print(json.dumps(report, indent=2), flush=True)
assert actual == latest, 'Public index/install is behind the stable release'
assert pin == latest, 'Bump the course pin and run the full first-use matrix for the new stable release'
