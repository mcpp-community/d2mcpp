#!/usr/bin/env python3
"""Run the README flow in a disposable checkout, with course downloads pinned to its commit.

Only Git's course URL is redirected to a local bare repository. xlings/d2x still
perform the real install. Checker flags automate saving and resuming in CI.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

from versions import verify

ROOT = Path(__file__).resolve().parents[2]
LOGS = ROOT / '.d2x/ci-logs'
WORK = ROOT / '.d2x/ci-work'
sys.stdout.reconfigure(encoding='utf-8', errors='replace')
sys.stderr.reconfigure(encoding='utf-8', errors='replace')


def code_blocks(text, language):
    return re.findall(r'```' + language + r'\n(.*?)```', text, re.S)


def readme_commands(name, start, end):
    text = (ROOT / name).read_text(encoding='utf-8').split(start, 1)[1].split(end, 1)[0]
    return [line.split('#', 1)[0].strip() for block in code_blocks(text, 'bash')
            for line in block.splitlines() if line.split('#', 1)[0].strip()]


commands = readme_commands('README.md', '### Practice locally', '## Community')
assert commands == readme_commands('README.zh.md', '### 搭建本地练习环境', '## 社区')
assert commands == readme_commands('README.zh.hant.md', '### 建立本地練習環境', '## 社區')
assert commands[-2:] == ['mcpp test -p solutions', 'd2x checker']
guide = (ROOT / 'docs/environment.md').read_text(encoding='utf-8')
translation = (ROOT / 'docs/environment.zh.md').read_text(encoding='utf-8')
for language in ['bash', 'powershell']:
    assert code_blocks(guide, language) == code_blocks(translation, language), 'Environment guide commands differ by language'
install = re.search(r'<a id="install-xlings"></a>\s*##[^\n]*\n(.*?)(?=\n## |\Z)', guide, re.S)[1]
bootstrap = code_blocks(install, 'powershell' if os.name == 'nt' else 'bash')[0]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--published', action='store_true', help='download the public course instead of the candidate fixture')
args = parser.parse_args()

assert not WORK.exists(), 'Use a fresh disposable checkout'
assert not (ROOT / '.d2x/state.json').exists(), 'Use a checkout without learning progress'
LOGS.mkdir(parents=True, exist_ok=True)
WORK.mkdir(parents=True)
source = WORK / 'source.git'
download = WORK / 'download'
download.mkdir()
head = subprocess.check_output(['git', '-C', str(ROOT), 'rev-parse', 'HEAD'], text=True).strip()
env = os.environ.copy()
env['XLINGS_NON_INTERACTIVE'] = '1'
if not args.published:
    subprocess.run(['git', 'clone', '--bare', '--no-hardlinks', str(ROOT), str(source)], check=True)
    subprocess.run(['git', '-C', str(source), 'update-ref', 'refs/heads/readme-ci', head], check=True)
    subprocess.run(['git', '-C', str(source), 'symbolic-ref', 'HEAD', 'refs/heads/readme-ci'], check=True)
    index = int(env.get('GIT_CONFIG_COUNT', '0'))
    for key, value in [(f'url.{source.as_uri()}.insteadOf', 'https://github.com/mcpp-community/d2mcpp.git'),
                       ('protocol.file.allow', 'always')]:
        env[f'GIT_CONFIG_KEY_{index}'] = key
        env[f'GIT_CONFIG_VALUE_{index}'] = value
        index += 1
    env['GIT_CONFIG_COUNT'] = str(index)

# All installation and verification commands come from the docs. The checker
# harness runs the last command with event output and simulates the user's save.
checker = ('python' if os.name == 'nt' else 'python3') + ' scripts/ci/first-run.py --allow-source-edit'
script = WORK / ('readme.ps1' if os.name == 'nt' else 'readme.sh')
if os.name == 'nt':
    body = "$ErrorActionPreference = 'Stop'\n" + bootstrap + '\n'
    for command in commands[:-1] + [checker]:
        if command == 'd2x install d2mcpp' and os.environ.get('D2X_CANDIDATE'):
            command = "& '" + os.environ['D2X_CANDIDATE'].replace("'", "''") + "' install d2mcpp"
        body += command + "\nif ($LASTEXITCODE -ne 0) { throw 'README command failed' }\n"
    script.write_text(body, encoding='utf-8')
    shell = ['powershell', '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', str(script)]
else:
    if os.environ.get('D2X_CANDIDATE'):
        import shlex
        commands = [shlex.quote(os.environ['D2X_CANDIDATE']) + ' install d2mcpp' if c == 'd2x install d2mcpp' else c for c in commands]
    script.write_text('set -euo pipefail\n' + bootstrap + '\n' + '\n'.join(commands[:-1] + [checker]) + '\n', encoding='utf-8')
    shell = ['bash', str(script)]

course = download / 'd2mcpp'
print('Running the README commands with the documented xlings installer', flush=True)
try:
    with (LOGS / 'readme-install.log').open('w', encoding='utf-8') as log:
        result = subprocess.run(shell, cwd=download, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=2700)
    output = (LOGS / 'readme-install.log').read_text(encoding='utf-8', errors='replace')
    print(output, flush=True)
    assert result.returncode == 0, 'README flow failed; see readme-install.log'
    verify(course, label='readme-selected-versions')
    expected = len(list((ROOT / 'solutions/tests').rglob('*.cpp')))
    assert expected > 0 and re.findall(r'(\d+) passed; (\d+) failed', output)[-1] == (str(expected), '0')
    assert f'FIRST-RUN PASS: {expected} exercises' in output
    actual = subprocess.check_output(['git', '-C', str(course), 'rev-parse', 'HEAD'], text=True).strip()
    if args.published:
        assert json.loads((course / '.xlings.json').read_text()) == json.loads((ROOT / '.xlings.json').read_text()), 'Published course pins are stale'
    else:
        assert actual == head, f'Course download used {actual}, expected candidate {head}'
    (LOGS / 'readme-result.json').write_text(json.dumps({'candidate': head, 'downloaded': actual, 'published': args.published, 'status': 'passed'}, indent=2), encoding='utf-8')
finally:
    if (course / '.d2x/ci-logs').exists():
        shutil.copytree(course / '.d2x/ci-logs', LOGS / 'readme-events', dirs_exist_ok=True)
if os.environ.get('GITHUB_PATH'):
    with open(os.environ['GITHUB_PATH'], 'a', encoding='utf-8') as paths:
        paths.write(str(Path.home() / '.xlings/subos/current/bin') + '\n')
print('README PASS: documented install, reference answers and save/advance/resume; course source verified')
