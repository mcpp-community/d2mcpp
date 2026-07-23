# Lesson Anatomy & Registration

The exact directory map, registration snippets, and verification commands for a
d2mcpp lesson. `<std>` is the standard section (`cpp11`, `cpp14`, …); `NN` is the
two-digit chapter number; `topic` is the kebab-case slug; `K` is the
sub-exercise index (`0`, `1`, …); `<name>` is the `d2x checker` name (the slug,
or `slug-1`, `slug-2` for later exercises).

## Directory map

```
book/src/<std>/NN-topic.md            # zh chapter
book/en/src/<std>/NN-topic.md         # en chapter
book/src/SUMMARY.md                   # zh TOC  (register here)
book/en/src/SUMMARY.md                # en TOC  (register here)
book/src/changelog.md                 # zh changelog (add entry)
book/en/src/changelog.md              # en changelog (add entry)

src/<std>/tests/NN-topic/K.cpp        # zh exercise(s)   — no registration needed
src/en/<std>/tests/NN-topic/K.cpp     # en exercise(s)   — no registration needed

solutions/<std>/NN-topic/K.cpp        # reference solution(s), zh/en 共用
```

Repo-level shared basis (NOT per-lesson, created once, never edited by hand):

```
msvc-stl/                             # vendored MSVC STL `stl/` subtree (read-only)
msvc-stl/stl/inc/<header>             # what the chapter's `## 二、真实案例` quotes from
msvc-stl/SOURCE.md                    # pinned upstream commit + refresh steps
msvc-stl/LICENSE.txt, NOTICE.txt      # upstream license, kept verbatim
```

The `## 二、真实案例` section of every chapter links into `msvc-stl/` for its
verbatim STL excerpt; you do not add files here per lesson — only refresh the
snapshot via `msvc-stl/SOURCE.md` when needed.

A new `<std>` section that does not exist yet needs: a `src/<std>/mcpp.toml` (copy
`src/cpp14/mcpp.toml`, change the name), `src/<std>/tests/` + `src/en/<std>/tests/` +
`solutions/<std>/` directories, the member added to the root `mcpp.toml`
workspace list (both `src/<std>` and `src/en/<std>`), and the section heading in both
`SUMMARY.md` files (e.g. `# C++14核心语言特性` / `# C++14 Core Language
Features`).

## Exercise registration — there is none

**Exercises are tests.** Each `<std>/` is a real mcpp project and exercises are
its `tests/`; dropping `<std>/tests/NN-topic/K.cpp` into place is the whole job.
`mcpp test -p src/<std>` runs them natively (the report is the progress table), and
the d2x Provider (`d2x/buildtools/`) derives the exercise id, order and
chapter from the same path — one chain, no generated manifests, nothing under
`.d2x/` but learner progress.

This is deliberate. rustlings' most expensive lesson (PR #1355) was the Rust
edition living in *two* places — the `rustc` args and `rust-project.json` —
which drifted and caused bugs. Any standalone registration file is a second
source of truth. Here the only truth is the directory layout.

### Per-exercise compile flags

When a lesson needs non-default flags, declare a per-glob entry in the member's
`<std>/mcpp.toml` — `[build].flags` globs cover test TUs:

```toml
[build]
flags = [
  { glob = "tests/NN-topic/*.cpp", cxxflags = ["-O0", "-fno-elide-constructors"] },
]
```

(No current exercise needs this; the capability exists for lessons that teach
observation-sensitive behavior.)

### C++ standard

All exercises compile as **c++23** (`standard` in each member's `mcpp.toml`).
The `cppNN/` directories denote *when a feature was introduced* — they do not
change compile flags.

## Solution registration — there is none either

Drop the file at `solutions/<std>/NN-topic/K.cpp` (zh/en share one solution).
CI swaps it over the exercise and asserts it passes; see
`d2x/buildtools/tests/e2e.sh`.

## SUMMARY registration

Add the chapter under the correct standard heading, keeping numeric order.

zh — `book/src/SUMMARY.md`:
```markdown
- [标题 - english name](./<std>/NN-topic.md)
```

en — `book/en/src/SUMMARY.md`:
```markdown
- [English Title](./<std>/NN-topic.md)
```

## Changelog entry

Add a dated entry to both `book/src/changelog.md` and `book/en/src/changelog.md`
under the current `## YYYY/MM` heading (create the month heading if missing).
Follow the existing entry shape — feature title, then `Book` / `Code` (and
`video` when available) links with per-artifact dates. Convert any relative date
to an absolute `YYYY/MM/DD`.

## Verification (definition of done)

Run from the project root; report real output, do not assert success blind:

```bash
# run the exercise natively (fastest loop while authoring)
mcpp test -p src/<std> NN-topic

# check it through the Provider path (exactly what `d2x checker` consumes)
mcpp run -q -p d2x/buildtools -- check cppNN-NN-topic-0

# or validate every exercise + solution at once (zh + en)
bash d2x/buildtools/tests/e2e.sh all

# drive the auto-checker against the exercise (expects the unsolved exercise to fail,
# the solution to pass) — name omits the NN- prefix
d2x checker <name>
```

Bilingual sync check: every file in the zh column has its en counterpart, code
bodies are identical between zh/en exercises, and both SUMMARY + both changelog
files were updated.
