<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../<std>/NN-topic.html
[English]: ./NN-topic.html

# English Title

<One-paragraph intro: this feature was introduced in C++NN to solve …, making …>

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/...) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/<std>/NN-topic.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/<std>/NN-topic-0.cpp) |  |


**Why was it introduced?**

- <motivation 1>
- <motivation 2>

**How does it differ from …?**

- <contrast 1>
- <contrast 2>

## I. Basic Usage and Scenarios

### <subsection title>

<explanation + short code blocks; comment the wrong vs. correct form inline>

```cpp
// example code
```

<!-- Every chapter should include this section; the code must be quoted verbatim
     from the in-repo msvc-stl/ basis (see the "Real-world case / msvc-stl"
     convention in SKILL.md). If a feature has no representative usage in the STL
     implementation, this section may be omitted with a reason noted in the PR. -->
## II. Real-World Case - <feature> in the STL

> Corroborates how the feature is actually used in production-grade code. The snippet is taken from the in-repo MSVC STL reference basis [`msvc-stl/`](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) (source file [`stl/inc/<header>`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/<header>)); `_NODISCARD` / `_CONSTEXPR17` / `_STD` are internal library macros — ignore them when reading

### <real-usage heading echoing a syntax point from section I>

<one line: why the standard library must use the feature here — un-writable / awkward by hand>

```cpp
// MSVC STL · stl/inc/<header> (abridged)
// <verbatim excerpt of the real code; mark omissions with (abridged), never paraphrase from memory>
```

<tie it back to the toy example in section I: you write it this way → the standard library writes it this way too — one of the core motivations for introducing the feature>

## III. Notes and Caveats

### <pitfall / migration advice>

<explanation>

<!-- Practice Topics list ALL exercises (-0, -1, …, matching the dslings/xmake count)
     as an index; but the checker block shows only the SINGLE entry command
     `d2x checker <name>` — d2x auto-advances through the rest, no need to list each.
     Each exercise's own checker name + a Tips line specific to that exercise live in
     that exercise file's header. The discussion link comes from the exercises'
     "练习交流讨论" header (usually one shared thread per chapter). -->
## IV. Practice Code

### Practice Topics

- 0 - [<exercise 0 topic>](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/<std>/NN-topic-0.cpp)
- 1 - [<exercise 1 topic>](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/<std>/NN-topic-1.cpp)

### Auto-Checker Commands

<!-- The tool-setup commands are fixed — copy this block verbatim. Put the <details>
     BEFORE the checker command (setup first, then the run command). Blank lines
     around the inner code fence are REQUIRED for mdBook to render it. -->
<details>
<summary>Don't have d2x yet? Click to expand setup</summary>

```bash
# 1. Install xlings (Linux / macOS)
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash
# Windows PowerShell:
# irm https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.ps1 | iex

# 2. Install d2x and fetch this tutorial
xlings install d2x -y
d2x install d2mcpp

# 3. Enter the project directory & run the checker
cd d2mcpp
```

</details>

```
d2x checker <name>
```

### Exercise Discussion

- [<this chapter's exercise thread>](https://forum.d2learn.org/post/NNN)

<!-- Optional: include only if the feature evolved in later standards. Pure
     refinements/deprecations belong here; independent new capabilities get
     their own chapter in the later standard's section, linked forward here.
     Placement: immediately before ## V. Additional Resources. -->
## Feature Evolution

> **Introduced in**: C++NN
> **Later improvements**:
> - **C++14**: <...>
> - **C++17**: <...> (see [the dedicated chapter](../<later-std>/MM-xxx.md))
> - **C++20**: <...>
>
> **Teaching note**: <cross-standard behavior differences / deprecation / why this exercise uses an exception standard>

## V. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repo](https://github.com/mcpp-community/d2mcpp)
- [mcpp-community Organization](https://github.com/mcpp-community)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Tooling - xlings](https://github.com/openxlings/xlings)
- [Exercise Checker - d2x](https://github.com/d2learn/d2x)
