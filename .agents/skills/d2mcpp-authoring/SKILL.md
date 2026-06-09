---
name: d2mcpp-authoring
description: >-
  Authoring conventions, design principles, and file formats for the d2mcpp
  (D2X) Modern C++ tutorial project. Use this whenever you add or edit a C++
  language-feature lesson — a book chapter, a dslings exercise, a reference
  solution, or when registering them in SUMMARY/xmake/changelog. Trigger it for
  any request like "add a chapter for X", "write a dslings exercise for fold
  expressions", "add the constexpr lesson", "fill the cpp14 section", or
  anything touching book/src, book/en/src, dslings/, or solutions/. Even small
  edits should follow these conventions so the bilingual + book/code/checker
  artifacts stay in sync.
---

# d2mcpp (D2X) Authoring Conventions

d2mcpp teaches **Modern C++ core language features** through a tightly coupled
quartet: **Book (mdBook) + Code (dslings exercises) + Solution + Auto-checker
(`d2x checker`)**, all of it **bilingual (zh + en)**. A "lesson" is never one
file — it is a coordinated set across several directories. The cardinal sin here
is producing a half-set (a chapter with no exercise, a zh file with no en
counterpart, an exercise not registered in xmake). This skill exists so every
lesson lands complete and consistent.

Read `references/anatomy.md` for the full directory map and the exact list of
files every lesson must touch. Use the ready-made scaffolds in `assets/` rather
than reconstructing format from memory.

## The design principle (decide this BEFORE writing)

The project is organized **by standard** (`cpp11/ cpp14/ cpp17/ cpp20/ cpp23/`).
Honor that axis:

- **Teach each feature as the standard that introduced it actually delivered
  it** — the historical state, compiled at its introduction standard. The
  compiler-driven (dslings) format is at its best when the learner feels the
  C++11-era restriction first (e.g. `constexpr` single-return, `std::is_pod`,
  copy-elision behavior). Folding every later refinement back into the
  introduction chapter erases that, and empties out the later-standard sections.
- **Independent later capabilities become their own chapter under the later
  standard's section** — this is what fills `cpp14/17/20`. Examples: fold
  expressions → cpp17, `using enum` → cpp20, designated initializers → cpp20,
  `if constexpr` → cpp17, `decltype(auto)` → cpp14, `consteval`/`constinit` → cpp20.
- **Pure refinements / rule-tweaks / deprecations stay in the introduction
  chapter as a short "特性演进 / Feature Evolution" note** with a cross-link
  forward — they are not chapter-worthy on their own. Examples: `auto x{1}`
  deduction change, `std::is_pod` deprecation pointer, guaranteed copy elision
  remark, P0136 inheriting-constructor semantics fix.
- **Compile each chapter at its introduction standard.** Do not globally pin a
  newer standard. If one exercise genuinely needs a later standard, make it an
  **explicit, commented exception** in xmake — never a bare `-- TODO` hack.

Background and the full per-feature evolution analysis live in
`.agents/docs/2026-06-08-cpp11-feature-evolution-and-cxx20-baseline.md`. Consult
it when deciding where a feature's later improvements should go.

The decision rule, compactly:

| Kind of later change | Where it goes |
|---|---|
| New independent syntax/ability | New chapter in the later standard's section |
| Relaxation/tweak/deprecation | "Feature Evolution" note in the intro chapter + forward link |

## The unit of work: one lesson = these files, all bilingual

When adding a feature numbered `NN` with slug `topic` (e.g. `06-scoped-enums`):

1. `book/src/<std>/NN-topic.md` — zh chapter
2. `book/en/src/<std>/NN-topic.md` — en chapter (translate prose, keep code identical)
3. `dslings/<std>/NN-topic-K.cpp` — one or more exercises (`K` = 0,1,2…), zh comments
4. `dslings/en/<std>/NN-topic-K.cpp` — en exercises (translate comments only)
5. `solutions/<std>/NN-topic-K.cpp` — reference solution per exercise
6. Register each exercise target in `dslings/<std>/xmake.lua`
7. Register each solution target in `solutions/xmake.lua`
8. Add the chapter line to **both** `book/src/SUMMARY.md` and `book/en/src/SUMMARY.md`
9. Add a changelog entry to **both** `book/src/changelog.md` and `book/en/src/changelog.md`

Missing any of these = an incomplete lesson. `references/anatomy.md` has the
exact registration snippets and SUMMARY/changelog line formats.

## Naming and numbering

- Chapter/exercise prefix is a **two-digit** sequence `NN` within the standard
  section; slug is **kebab-case** and matches book + dslings + solutions.
- Exercises split into `-0`, `-1`, … by sub-topic; a single-exercise chapter has
  no numeric suffix on the dslings target name but the file is still `NN-topic.cpp`.
- The **`d2x checker <name>`** name is the slug (and `<slug>-1`, `<slug>-2` for
  later exercises) — it omits the `NN-` prefix. Keep checker names, file names,
  and book references mutually consistent.

## Book chapter format

Use `assets/chapter.zh.md` / `assets/chapter.en.md`. Required structure, in order:

1. Language-switch header `<div align=right> 🌎 [中文] | [English]` + link defs.
   - zh links: `[中文]: ./NN-topic.html` and `[English]: ../en/<std>/NN-topic.html`
   - en links: `[中文]: ../../<std>/NN-topic.html` and `[English]: ./NN-topic.html`
2. `# 标题 - english name` (zh) / `# English Title` (en), then a one-paragraph
   intro that states the feature **was introduced in C++NN** and what problem it solves.
3. Resource table `| Book | Video | Code | X |` — cppreference link + markdown
   permalink + video link + first practice-code permalink.
4. Bold motivation blocks: `**为什么引入?**`, `**和……的区别?**`.
5. Numbered Chinese-numeral sections, in order: `## 一、基础用法和场景`,
   `## 二、真实案例 - STL 中的 <feature>`, `## 三、注意事项`,
   `## 四、练习代码`, `## 五、其他`.
6. `## 二、真实案例` corroborates the feature with **real, verbatim STL code**
   quoted from the in-repo `msvc-stl/` basis — see "The 真实案例 section" below.
7. `## 四、练习代码` lists **every** exercise topic of the chapter (`-0`, `-1`, …,
   matching the dslings/xmake count) with dslings links as an index, but the checker
   block shows only the **single entry** command `d2x checker <name>` — `d2x` auto-
   advances through the remaining exercises, so do NOT list one command per exercise.
   Then a `### 练习交流讨论` link to the chapter's forum thread. Each exercise's own
   `d2x checker <name>-K` name and a Tips line **specific to that exercise** belong in
   that exercise file's header, not in the book.
8. `## 五、其他` is the fixed footer (forum / repo / video list / xlings links).
9. If the feature evolved later, add a **`## 特性演进 / Feature Evolution`**
   block before `## 五、其他` — introduced standard + bullet list of later
   refinements + forward links to the later-standard chapters.

Keep prose tone consistent with existing chapters: explanatory, example-driven,
short code blocks with inline comments showing the failure and the fix.

### The 真实案例 / Real-World Case section (msvc-stl basis)

`## 二、真实案例` exists to prove the feature earns its place by showing **real
production code, quoted verbatim** — never invented, never paraphrased. The single
source of truth is the in-repo vendored MSVC STL basis at the top-level
**`msvc-stl/`** (the complete upstream `stl/` subtree; `msvc-stl/SOURCE.md` records
the pinned upstream commit and how to refresh it). Rules:

- Quote from `msvc-stl/stl/inc/<header>` (or `src/`) **byte-for-byte**; mark any
  elision with `(有删节)` / `(abridged)`. Do not reconstruct from memory.
- Keep internal macros (`_NODISCARD`, `_CONSTEXPR17`, `_STD`, …) as written and add
  one line telling the reader they are library-internal and ignorable.
- Pick a usage that **echoes a syntax point from `## 一`** (e.g. `std::begin`'s
  `auto … -> decltype(...)` for the auto/decltype chapter), so the section reads as
  "you use it this way → the standard library does too".
- **Scope-lock to the chapter's standard**: a `cpp11` chapter quotes C++11-era
  usage only; do not show later forms (e.g. `decltype(auto)`).
- Link to the **local `msvc-stl/` path**, not the upstream GitHub repo, so the
  citation stays pinned to the vendored snapshot.
- Every chapter should carry this section. If a feature genuinely has no
  representative usage in the STL implementation, omit it and state why in the PR.

## dslings exercise format

Use `assets/exercise.cpp`. Essentials:

- Header comment block: `d2mcpp` URL, `license: Apache-2.0`, `file:` path,
  `Exercise/练习:` line (`<std> | NN - topic | 中文小标题`), `Tips/提示:`,
  `Docs/文档:` (cppreference), and the `Auto-Checker/自动检测命令:` with
  `d2x checker <name>`.
- `#include <d2x/cpp/common.hpp>` then any std headers.
- `main()` seeded with **intentional errors** the learner fixes, each flagged by
  a numbered inline comment (`// 1.…`, `// 2.…`) telling them what to do.
- Checkpoint macros from `d2x/cpp/common.hpp`:
  - `d2x_assert(cond)` / `d2x_assert_eq(a, b)` — runtime checkpoints; learners
    must make them pass by fixing code, **not** by deleting the checkpoint.
  - `D2X_YOUR_ANSWER` — a fill-in placeholder the learner replaces.
  - `D2X_WAIT` — separates exercises; learner deletes/comments it to advance.
    Put exactly one at the end of each exercise's checkpoints.
  - `D2X_DONT_DELETE_THIS` — guards lines that must not be removed.
- The **en exercise translates only the comment prose**; the code stays byte-for-byte
  identical to the zh exercise so checker behavior matches.

## Reference solution format

Use `assets/solution.cpp`. It is the corrected exercise with:

- A solution header: `reference solution for: <exercise path>`, plus the note
  that it is **for CI/maintainers only, not a tutorial entry**, and a pointer
  back to the exercise file.
- `D2X_WAIT` removed; all checkpoints passing; unused locals silenced with
  `(void)var;` rather than left to warn.
- Same `#include`s and structure as the exercise.

## Definition of done

Before claiming a lesson complete, verify the whole set exists and is wired up,
and that the exercise/solution actually build & check. See
`references/anatomy.md` for the verification commands (`d2x checker <name>` and
the xmake build) — run them; do not assert success without the output.
