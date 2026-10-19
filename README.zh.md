<div align=right>

  [中文] | [繁體中文] | [English] | [Todo]
</div>

<div align=center>
  <img width="320" src="https://github.com/user-attachments/assets/dcc6bdea-71f8-4ed5-b445-6449c88655f3">

  <em>动手学 | 现代CPP核心语言特性 - <b>"强调动手实践的C++教程项目"</b></em>

  <b> [📚Book] + [🎥Video] + [⌨️Code] + [👥X] </b>
</div>

[中文]: README.zh.md
[繁體中文]: README.zh.hant.md
[English]: README.md
[Todo]: README.md

[📚Book]: https://mcpp-community.github.io/d2mcpp
[🎥Video]: https://www.bilibili.com/video/BV182MtzPEiX
[⌨️Code]: https://github.com/mcpp-community/d2mcpp/tree/main/src
[👥X]: https://forum.d2learn.org/category/20

> [!CAUTION]
> 该项目当前处于开发初期, 如果你发现任何问题, 欢迎创建issues反馈或直接提交PR进行修复

## 目标

- **`[掌握]`** - **现代C++核心的语言特性**及使用场景
- **`[掌握]`** - 通过编译器报错信息**定位问题的能力**
- **`[熟悉]`** - 通过文档和[cppreference](https://cppreference.com)解决C++中不熟悉问题的能力
- **`[了解]`** - 如何参与技术社区 - 开源项目的使用、提问题、参与讨论或贡献

## 快速开始

> 尝试 `Code -> Book -> Video -> X -> Code`

### 在线代码练习

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=mcpp-community/d2mcpp)

等待环境准备完成（首次启动会下载工具链并验证参考答案），然后在终端运行 `d2x checker`，或使用 **Tasks: Run Task → d2mcpp: practice**。重连不会重复启动 checker。准备失败时查看[环境排错说明](docs/environment.md)。

### 搭建本地练习环境

支持的发布平台：Linux x86_64、macOS 14+ Apple Silicon、Windows x86_64（Windows PowerShell 5.1+）。mcpp 管理 C++ 工具链，Visual Studio 为可选项。当前课程所用 d2x Release 尚未覆盖 Linux ARM64。编辑器可自行选择，课程不会强制安装。

**第一步：获取课程**（建议先 fork，用 git 保存自己的练习进度）：

```bash
git clone https://github.com/mcpp-community/d2mcpp.git
cd d2mcpp
```

**第二步：准备环境。** 脚本安装 xlings **2026.10.18.1** 及 `.xlings.json` 声明的版本（mcpp **2026.10.17.1**、d2x **2026.08.02.2**、mdBook **0.4.43**），构建并检查 Provider。首次构建可能需要几分钟。

Linux / macOS：

```bash
bash scripts/setup.sh
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
```

Windows PowerShell：

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\setup.ps1
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
```

PATH 命令让当前终端立即找到工具；安装后重新打开终端即可供后续会话使用。准备脚本可重复执行，不会改动练习文件或学习进度。

<details>
<summary>可选：通过 d2x install 下载课程</summary>

先安装 [xlings](https://github.com/openxlings/xlings)：

```bash
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/v2026.10.18.1/tools/other/quick_install.sh | bash -s v2026.10.18.1
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
```

Windows PowerShell 使用：

```powershell
irm https://raw.githubusercontent.com/mcpp-community/d2mcpp/main/scripts/install-xlings.ps1 | iex
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
```

然后在任一系统执行：

```text
xlings install d2x@2026.08.02.2 -y
d2x install d2mcpp
cd d2mcpp
```

继续执行第二步中对应系统的准备脚本；两种下载方式共用下面的验证步骤。

</details>

**第三步：验证环境并开始练习：**

```bash
mcpp test -p solutions       # 参考答案应全部通过（当前 52 份）
d2x checker                  # 编辑 -> 保存 -> 自动检测 -> 推进
d2x status                   # 进度总览
```

未完成的练习报错是预期行为，参考答案必须全绿。`Provider` 错误或链接路径含 `/nonexistent/` 属于环境问题，请查看[诊断与恢复步骤](docs/environment.md)。`git pull` 后重新运行准备脚本，让更新后的版本配置生效。

也可使用 `mcpp test -p src/cpp11` 直接练习某一章。更多操作见[使用说明](https://mcpp-community.github.io/d2mcpp/base/chapter_1.html)。


## 社区

- **即时交流:** 167535744、1067245099
- [**论坛版块:**](https://forum.d2learn.org/category/20) 问题反馈、练习代码、技术交流和讨论
- **社区活动:** [📣 MSCP - mcpp项目学习与贡献者培养计划](https://moga.d2learn.org/activity/mscp/intro.html)

> **注:** 复杂性问题(技术、环境搭建等问题)推荐在论坛发帖, 并详细描述问题细节, 能更有效于问题的解决和复用

## 参与贡献

- **参与社区交流:** 反馈问题、参与社区问题讨论、帮助社区新用户解决问题
- **参与项目维护和开发:** 参与社区中问题处理、修复Bug、多语言支持、[加入MSCP活动小组](https://moga.d2learn.org/activity/mscp/docs/join-group.html)、开发&优化新功能/模块

**📑开源协议与贡献许可(License & CLA)**

- 本项目欢迎自由使用与分发！你可以在 [Apache License 2.0](LICENSE-CODE) 和 [CC-BY-NC-SA 4.0](LICENSE-BOOK) 协议下**免费**使用、修改和分享本项目的代码与文档内容
- 如希望参与贡献代码或文档，请先阅读[贡献者许可协议（CLA）](CLA.md)

**👥贡献者**

[![Star History Chart](https://api.star-history.com/svg?repos=mcpp-community/d2mcpp&type=date&legend=top-left)](https://www.star-history.com/#mcpp-community/d2mcpp&type=date&legend=top-left)

<a href="https://github.com/mcpp-community/d2mcpp/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=mcpp-community/d2mcpp" />
</a>

<a href="https://hellogithub.com/en/repository/mcpp-community/d2mcpp" target="_blank"><img src="https://api.hellogithub.com/v1/widgets/recommend.svg?rid=7877f7cb12e940a5a432d49c19a360df&claim_uid=aNLTSv91Awj8ruX&theme=dark" alt="Featured｜HelloGitHub" style="width: 250px; height: 54px;" width="250" height="54" /></a>
