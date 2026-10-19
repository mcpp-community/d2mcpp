#!/usr/bin/env python3
"""Exercise the real checker, including a save and a restart, in a disposable checkout."""
import argparse
import json
import os
from pathlib import Path
import queue
import signal
import subprocess
import sys
import threading
import time

ROOT = Path(__file__).resolve().parents[2]
LOGS = ROOT / '.d2x' / 'ci-logs'
sys.stdout.reconfigure(encoding='utf-8', errors='replace')
sys.stderr.reconfigure(encoding='utf-8', errors='replace')


def run(*args, name, timeout=1800):
    LOGS.mkdir(parents=True, exist_ok=True)
    result = subprocess.run(args, cwd=ROOT, text=True, encoding='utf-8', errors='replace',
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout)
    (LOGS / (name + '.stdout')).write_text(result.stdout, encoding='utf-8')
    (LOGS / (name + '.stderr')).write_text(result.stderr, encoding='utf-8')
    if result.returncode:
        raise AssertionError(f'{args} exited {result.returncode}\n{result.stderr[-8000:]}\n{result.stdout[-8000:]}')
    return [json.loads(line) for line in result.stdout.splitlines() if line.startswith('{')]


class Checker:
    def __init__(self, name, timeout):
        LOGS.mkdir(parents=True, exist_ok=True)
        self.events = queue.Queue()
        self.timeout = timeout
        self.process = subprocess.Popen(
            ['d2x', 'checker', '--ui', 'print', '--emit-events'], cwd=ROOT,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
            encoding='utf-8', errors='replace', bufsize=1,
            start_new_session=os.name != 'nt',
            creationflags=subprocess.CREATE_NEW_PROCESS_GROUP if os.name == 'nt' else 0)
        self.threads = []
        for stream, suffix in [(self.process.stdout, 'ndjson'), (self.process.stderr, 'stderr')]:
            thread = threading.Thread(target=self.read, args=(stream, LOGS / f'{name}.{suffix}', suffix), daemon=True)
            thread.start()
            self.threads.append(thread)

    def read(self, stream, path, suffix):
        with path.open('w', encoding='utf-8') as log:
            for line in stream:
                log.write(line)
                log.flush()
                if suffix == 'ndjson' and line.startswith('{'):
                    try:
                        self.events.put(json.loads(line))
                    except json.JSONDecodeError:
                        self.events.put({'event': 'invalid-json', 'line': line})

    def until(self, predicate):
        deadline = time.monotonic() + self.timeout
        while time.monotonic() < deadline:
            try:
                event = self.events.get(timeout=0.25)
            except queue.Empty:
                if self.process.poll() is not None:
                    raise AssertionError(f'checker exited early ({self.process.returncode}); see {LOGS}')
                continue
            if event.get('event') in ('invalid-json', 'error', 'done'):
                raise AssertionError(f'Unexpected checker event: {event}')
            if predicate(event):
                return event
        raise AssertionError(f'checker did not reach the expected state in {self.timeout}s; see {LOGS}')

    def close(self):
        if self.process.poll() is None:
            if os.name == 'nt':
                subprocess.run(['taskkill', '/PID', str(self.process.pid), '/T', '/F'], capture_output=True)
            else:
                os.killpg(self.process.pid, signal.SIGTERM)
        self.process.wait(timeout=30)
        for thread in self.threads:
            thread.join(timeout=5)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--allow-source-edit', action='store_true', required=True,
                        help='acknowledge this is a disposable checkout; the first exercise is temporarily replaced')
    parser.add_argument('--cold', action='store_true')
    parser.add_argument('--timeout', type=int, default=900)
    args = parser.parse_args()
    os.chdir(ROOT)
    state_path = ROOT / '.d2x/state.json'
    assert not state_path.exists(), 'Use a disposable checkout with no learning progress'
    if args.cold:
        assert not (ROOT / 'target').exists(), 'Provider must not have been built'
        assert not (ROOT / 'd2x/buildtools/target').exists(), 'Provider must not have been built'
        home = Path(os.environ['MCPP_HOME'])
        assert not home.exists() or not any(home.iterdir()), 'Cold test requires an empty MCPP_HOME'
    config = json.loads((ROOT / '.d2x.json').read_text(encoding='utf-8'))
    prefix = ROOT / 'src' / ('en' if config.get('lang') == 'en' else '')
    expected = sum(len(list((member / 'tests').rglob('*.cpp'))) for member in prefix.iterdir()
                   if member.is_dir() and (member.name == 'intro' or member.name.startswith('cpp')))
    assert expected > 1
    exercise = prefix / 'intro/tests/hello-mcpp.cpp'
    original = exercise.read_bytes()
    start = time.monotonic()
    checker = Checker('first-start', args.timeout)
    try:
        session = checker.until(lambda e: e.get('event') == 'session')
        assert session['total'] == expected and session['completed'] == 0, session
        first = checker.until(lambda e: e.get('event') == 'exercise')
        assert first['id'] == 'hello-mcpp', first
        verdict = checker.until(lambda e: e.get('event') == 'verdict')
        assert verdict['outcome'] == 'fail' and verdict['stage'] == 'compile', verdict
        checker.until(lambda e: e.get('event') == 'waiting' and e.get('reason') == 'file-change')
        print(f'First exercise reached in {time.monotonic() - start:.1f}s', flush=True)
        # FileWatcher resync follows the waiting event. Let it finish before the save.
        time.sleep(1)
        exercise.write_bytes((ROOT / 'solutions/tests/intro/hello-mcpp.cpp').read_bytes())
        verdict = checker.until(lambda e: e.get('event') == 'verdict')
        assert verdict['outcome'] == 'pass', verdict
        next_exercise = checker.until(lambda e: e.get('event') == 'exercise')
        assert next_exercise['id'] != 'hello-mcpp', next_exercise
        checker.until(lambda e: e.get('event') == 'waiting')
    finally:
        checker.close()
        exercise.write_bytes(original)
    state = json.loads(state_path.read_text(encoding='utf-8'))
    assert 'hello-mcpp' in state['completed'], state
    before_setup = state_path.read_bytes()
    if os.name == 'nt':
        run('powershell', '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', 'scripts/setup.ps1', name='repeat-setup')
    else:
        run('bash', 'scripts/setup.sh', name='repeat-setup')
    assert state_path.read_bytes() == before_setup, 'setup changed learning progress'
    checker = Checker('restart', args.timeout)
    try:
        session = checker.until(lambda e: e.get('event') == 'session')
        assert session['completed'] == 1, session
        current = checker.until(lambda e: e.get('event') == 'exercise')
        assert current['id'] == next_exercise['id'], current
        checker.until(lambda e: e.get('event') == 'waiting')
    finally:
        checker.close()
    description = run('mcpp', 'run', '-q', '-p', 'd2x/buildtools', '--', 'describe', name='describe')
    assert {'event': 'describe', 'name': 'mcpp', 'protocol': 1} in description, description
    exercises = run('mcpp', 'run', '-q', '-p', 'd2x/buildtools', '--', 'exercises', name='exercises')
    ids = [e['id'] for e in exercises if e.get('event') == 'exercise']
    assert len(ids) == len(set(ids)) == expected and all(ids), ids
    print(f'FIRST-RUN PASS: {expected} exercises; failure -> save -> pass -> advance -> restart; setup preserves progress')


if __name__ == '__main__':
    main()
