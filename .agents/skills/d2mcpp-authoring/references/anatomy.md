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

dslings/<std>/NN-topic-K.cpp          # zh exercise(s)
dslings/en/<std>/NN-topic-K.cpp       # en exercise(s)
dslings/<std>/xmake.lua               # register exercise target(s)

solutions/<std>/NN-topic-K.cpp        # reference solution(s)
solutions/xmake.lua                   # register solution target(s) (includes <std>)
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

A new `<std>` section that does not exist yet also needs: an `includes("<std>")`
line wired from `dslings/xmake.lua` and `solutions/xmake.lua`, a `dslings/<std>/`
and `dslings/en/<std>/` directory, a `solutions/<std>/` directory, and the
section heading in both `SUMMARY.md` files (e.g. `# C++14核心语言特性` /
`# C++14 Core Language Features`).

## xmake registration — exercises (`dslings/<std>/xmake.lua`)

Append one target per exercise file. Binary kind is the default; only set it
where existing siblings do. Example for a two-exercise chapter:

```lua
-- target: <std>-NN-topic

target("<std>-NN-topic-0")
    add_files("NN-topic-0.cpp")

target("<std>-NN-topic-1")
    add_files("NN-topic-1.cpp")
```

Per-target options seen in the repo, use only when the lesson needs them:

```lua
target("cpp11-04-rvalue-references")
    set_optimize("none")
    add_cxxflags("-fno-elide-constructors")   -- observe moves; note C++17 guaranteed elision can't be disabled
    add_files("04-rvalue-references.cpp")
```

If an exercise needs a non-default standard, make it an **explicit, commented
exception** — never a bare TODO:

```lua
target("cppNN-NN-topic-0")
    set_languages("c++17")   -- exception: <one-line reason this lesson needs c++17>
    add_files("NN-topic-0.cpp")
```

The default standard is set at the top of `dslings/<std>/xmake.lua`. Per the
design principle, that should be the **introduction standard** of the section
(e.g. `set_languages("c++11")` for `cpp11`). Avoid per-host standard forks; if
one is unavoidable, comment why.

## xmake registration — solutions (`solutions/xmake.lua` + `solutions/<std>/xmake.lua`)

Mirror the exercise targets so CI builds the reference solutions too. Follow the
existing pattern in `solutions/<std>/xmake.lua`.

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
# build the exercise + solution targets (lang defaults come from xmake)
xmake f -c >/dev/null && xmake build cppNN-NN-topic-0

# drive the auto-checker against the exercise (expects the unsolved exercise to fail,
# the solution to pass) — name omits the NN- prefix
d2x checker <name>
```

Bilingual sync check: every file in the zh column has its en counterpart, code
bodies are identical between zh/en exercises, and both SUMMARY + both changelog
files were updated.
