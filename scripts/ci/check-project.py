#!/usr/bin/env python3
"""Check course parity, shared version declarations, and local documentation links."""
import json
from pathlib import Path
import re
from urllib.parse import unquote

ROOT = Path(__file__).resolve().parents[2]
workspace = json.loads((ROOT / '.xlings.json').read_text())['workspace']
assert workspace['mcpp'] == '2026.10.17.1' and isinstance(workspace['mcpp'], str)
assert 'code' not in workspace
solutions = {p.relative_to(ROOT / 'solutions/tests') for p in (ROOT / 'solutions/tests').rglob('*.cpp')}
assert solutions
for prefix in [ROOT / 'src', ROOT / 'src/en']:
    exercises = {Path(member.name) / p.relative_to(member / 'tests')
                 for member in prefix.iterdir() if member.name == 'intro' or member.name.startswith('cpp')
                 for p in (member / 'tests').rglob('*.cpp')}
    assert exercises == solutions, (prefix, exercises ^ solutions)

docs = [ROOT / name for name in ['README.md', 'README.zh.md', 'README.zh.hant.md',
        'book/src/base/chapter_1.md', 'book/en/src/base/chapter_1.md',
        'docs/environment.md', 'docs/environment.zh.md']]
version = re.search(r'^XLINGS_VERSION=(.+)$', (ROOT / 'scripts/tool-versions.env').read_text(), re.M)[1]
for doc in docs:
    text = doc.read_text(encoding='utf-8')
    if not doc.name.startswith('README'):
        assert 'scripts/setup.' in text, f'Missing preparation command in {doc}'
    assert 'd2x checker' in text
    assert 'tree/main/dslings' not in text, f'Obsolete code link in {doc}'
    if 'chapter_1' in doc.name:
        assert version in text, f'Installer version differs in {doc}'
    # Code fences contain literal examples, not navigable Markdown links.
    prose = re.sub(r'```.*?```', '', text, flags=re.S)
    links = re.findall(r'\]\(([^)\s]+)\)', prose) + re.findall(r'^\[[^\]]+\]:\s*(\S+)', prose, re.M)
    for link in links:
        if re.match(r'^[a-zA-Z][a-zA-Z0-9+.-]*:', link) or link.startswith(('#', '/')):
            continue
        target = doc.parent / unquote(link.split('#')[0])
        if target.suffix == '.html' and 'book/' in str(doc):
            output = ROOT / 'book/book'
            page = output / ('en/base' if '/en/' in str(doc) else 'base')
            relative = (page / unquote(link.split('#')[0])).resolve().relative_to(output)
            if relative.parts[0] == 'en':
                target = ROOT / 'book/en/src' / Path(*relative.parts[1:])
            else:
                target = ROOT / 'book/src' / relative
            target = target.with_suffix('.md')
        assert target.exists(), f'Broken local link: {doc.relative_to(ROOT)} -> {link}'
print(f'PROJECT PASS: {len(solutions)} paired exercises per language; configuration and local documentation links valid')
