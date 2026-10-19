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
[⌨️Code]: https://github.com/mcpp-community/d2mcpp/tree/main/src
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

### Practice online

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=mcpp-community/d2mcpp)

Wait for environment preparation to finish (the first launch downloads a toolchain and verifies the reference answers). Then open a terminal and run `d2x checker`, or choose **Tasks: Run Task → d2mcpp: practice**. Reconnecting does not start another checker. If preparation fails, see [environment troubleshooting](docs/environment.md).

### Practice locally

Supported release targets: Linux x86_64, macOS 14+ Apple Silicon, and Windows x86_64 with Windows PowerShell 5.1+. mcpp manages the C++ toolchain; Visual Studio is optional. Linux ARM64 is not yet covered by this course's d2x release. Use your preferred editor; it is not installed as a course dependency.

**1. Get the course** (fork first if you want to save progress with git):

```bash
git clone https://github.com/mcpp-community/d2mcpp.git
cd d2mcpp
```

**2. Prepare the environment.** These scripts install xlings **2026.10.18.1**, the versions in `.xlings.json` (mcpp **2026.10.17.1**, d2x **2026.08.02.2**, mdBook **0.4.43**), and build and check the Provider. The first build may take several minutes.

Linux / macOS:

```bash
bash scripts/setup.sh
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
```

Windows PowerShell:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\setup.ps1
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
```

The PATH command makes the installed tools available in this terminal. For future sessions, reopen your terminal after installation. Preparation can be repeated and preserves your exercise files and progress.

<details>
<summary>Alternative: download with d2x install</summary>

Install [xlings](https://github.com/openxlings/xlings) first:

```bash
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/v2026.10.18.1/tools/other/quick_install.sh | bash -s v2026.10.18.1
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
```

Or on Windows PowerShell:

```powershell
irm https://raw.githubusercontent.com/mcpp-community/d2mcpp/main/scripts/install-xlings.ps1 | iex
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
```

Then, on either system:

```text
xlings install d2x@2026.08.02.2 -y
d2x install d2mcpp
cd d2mcpp
```

Continue with the preparation script for your system in step 2; both download routes use the same verification below.

</details>

**3. Verify, then practice:**

```bash
mcpp test -p solutions       # all reference answers must pass (currently 52)
d2x checker                  # edit -> save -> auto-check -> advance
d2x status                   # progress overview
```

The unfinished exercises are expected to fail; the reference answers must pass. `Provider` errors and linker paths containing `/nonexistent/` indicate an environment problem: see [diagnosis and recovery](docs/environment.md). After `git pull`, rerun the preparation script to apply any changed pins.

For native practice, `mcpp test -p src/cpp11` runs one section. See the [English usage guide](https://mcpp-community.github.io/d2mcpp/en/base/chapter_1.html).


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
