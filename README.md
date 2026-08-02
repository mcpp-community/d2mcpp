<div align=right>

  [中文] | [繁體中文] | [English] | [Todo]
</div>

<div align=center>
  <img width="320" src="https://github.com/user-attachments/assets/dcc6bdea-71f8-4ed5-b445-6449c88655f3">

  <em>D2X | Modern C++ Core Language Features - <b>"A C++ tutorial project focused on practical"</b></em>

  <b> [📚Book] + [🎥Video] + [⌨️Code] + [👥X] </b>
</div>

[中文]: README.zh.md
[繁體中文]: README.zh.hant.md
[English]: README.md
[Todo]: README.md

[📚Book]: https://mcpp-community.github.io/d2mcpp/en/index.html
[🎥Video]: https://youtube.com/playlist?list=PL7uow6t1QjF0ooMLkLSS96swpSuBZvoRE&si=1xHOGVIYpbzZAosI
[⌨️Code]: https://github.com/mcpp-community/d2mcpp/tree/main/dslings
[👥X]: https://forum.d2learn.org/category/20

## Goals

- **`[Master]`** - **Core language features of Modern C++** and their usage scenarios
- **`[Master]`** - The ability to **identify and debug issues** using compiler error messages
- **`[Familiarize]`** - The ability to solve unfamiliar C++ problems using documentation and [cppreference](https://cppreference.com)
- **`[Understand]`** - How to participate in the technical community — using open-source projects, asking questions, joining discussions, or contributing

> [!CAUTION]
> This project is currently in its early development stage. If you encounter any issues, feel free to create an issue for feedback or directly submit a PR to fix it.

## Quick Start

> Try `Code -> Book -> Video -> X -> Code`

### Interactive Code Practice (Online)

> [**click the button below**](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=mcpp-community/d2mcpp) — the cloud environment configures itself (a few minutes on first launch), then the exercise checker starts automatically

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=mcpp-community/d2mcpp)

### Interactive Code Practice (Local)

**Step 1 - Install the [d2x](https://github.com/d2learn/d2x) and [mcpp](https://github.com/mcpp-community/mcpp) tools**

<details>
  <summary>click to view xlings installation command</summary>

---

#### Linux/MacOS

```bash
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash
```

#### Windows - PowerShell

```bash
irm https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.ps1 | iex
```

> tips: xlings -> [details](https://xlings.d2learn.org)

---

</details>

```bash
xlings install d2x mcpp -y   # d2x: exercise framework CLI | mcpp: C++ build & test tool
```

**Step 2 - Get the course, then configure and verify the environment**

```bash
d2x install d2mcpp           # download the course, toolchain auto-configured
cd d2mcpp
mcpp test                    # environment self-check — prints your progress table
```

> The exercises **are** the tests, and they ship unfinished — so a fresh `mcpp test` is
> *supposed* to come out all red. What it verifies is everything around them: the
> toolchain resolved, the `d2x` library built, and the runner reached every exercise.
> Copy `solutions/` over the exercise files and the very same command goes all green
> (that is exactly what CI does).

<details>
  <summary>optional - start from the git source instead</summary>

---

```bash
git clone https://github.com/mcpp-community/d2mcpp.git   # or your fork
cd d2mcpp
xlings install -y            # toolchain pinned by .xlings.json
mcpp test                    # same environment check
```

> Tip: fork this repo and clone your fork — `git commit / push` keeps your practice progress in your own repository.

---

</details>

**Step 3 - Start practicing**

```bash
d2x checker                  # practice loop: edit -> save -> auto-check -> advance
d2x status                   # progress overview
```

> Want to run a single chapter without d2x? `mcpp test -p src/cpp11` works too.

**👉 [more details...](https://mcpp-community.github.io/d2mcpp/base/chapter_1.html)**

## Community

- **groups:** [mcpp forum](https://forum.d2learn.org/category/20)
- [**forum:**](https://forum.d2learn.org/category/20) issues feedback, practice code, technical discussions
- **community activities:** [📣 MSCP - mcpp project learning and contributor training program](https://moga.d2learn.org/activity/mscp/intro.html)

> **Note:** Complex issues (technical, environment setup, etc.) are recommended to be posted on the forum and detailed description of the problem can be more effective in problem solving and reuse.

## Contributing

- **Community Communication:** Report issues, participate in community discussions, and help new users solve problems.
- **Project Maintenance and Development:** Participate in community issue resolution, bug fixes, multilingual support, [join the MSCP activity group](https://moga.d2learn.org/activity/mscp/docs/join-group.html), and develop and optimize new features and modules.

**📑License & CLA**

- This project welcomes free use and distribution! You may use, modify, and share the code and documentation in this project **free** under the [Apache License 2.0](LICENSE-CODE) and [CC-BY-NC-SA 4.0](LICENSE-BOOK) licenses.
- If you would like to contribute code or documentation, please read the [Contributor License Agreement (CLA)](CLA.md) first.

**👥Contributors**

[![Star History Chart](https://api.star-history.com/svg?repos=mcpp-community/d2mcpp&type=date&legend=top-left)](https://www.star-history.com/#mcpp-community/d2mcpp&type=date&legend=top-left)

<a href="https://github.com/mcpp-community/d2mcpp/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=mcpp-community/d2mcpp" />
</a>

<a href="https://hellogithub.com/en/repository/mcpp-community/d2mcpp" target="_blank"><img src="https://api.hellogithub.com/v1/widgets/recommend.svg?rid=7877f7cb12e940a5a432d49c19a360df&claim_uid=aNLTSv91Awj8ruX&theme=dark" alt="Featured｜HelloGitHub" style="width: 250px; height: 54px;" width="250" height="54" /></a>
