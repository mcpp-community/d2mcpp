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
[⌨️Code]: https://github.com/mcpp-community/d2mcpp/tree/main/dslings
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

> [**点击下面按钮**](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=mcpp-community/d2mcpp) —— 云端自动完成环境配置(首次启动需几分钟), 随后自动进入练习检测模式

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=mcpp-community/d2mcpp)

### 搭建本地练习环境

**第一步 - 安装 [d2x](https://github.com/d2learn/d2x) 和 [mcpp](https://github.com/mcpp-community/mcpp) 工具**

<details>
  <summary>点击查看xlings安装命令</summary>

---

#### Linux/MacOS

```bash
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash
```

#### Windows - PowerShell

```bash
irm https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.ps1 | iex
```

> 注: xlings工具 -> [详情](https://xlings.d2learn.org)

---

</details>

```bash
xlings install d2x mcpp -y   # d2x: 练习框架 CLI | mcpp: C++ 构建与测试工具
```

**第二步 - 获取课程, 并配置和验证环境**

```bash
d2x install d2mcpp           # 下载课程,环境自动配置
cd d2mcpp
mcpp test                    # 环境自检 —— 打印你的进度表
```

> 练习**就是**测试, 而且发下来时是没做完的 —— 所以刚拿到课程时 `mcpp test` **本来就该全红**。
> 它验的是练习之外的一切: 工具链解析成功、`d2x` 库编译通过、测试运行器逐题都跑到了。
> 把 `solutions/` 覆盖到练习文件上, 同一条命令就会全绿(CI 做的正是这件事)。

<details>
  <summary>可选 - 改用 clone 项目源码的方式</summary>

---

```bash
git clone https://github.com/mcpp-community/d2mcpp.git   # 或你 fork 后的仓库
cd d2mcpp
xlings install -y            # 按 .xlings.json 安装固定版本工具链
mcpp test                    # 同样的环境验证
```

> 建议: 先 fork 本仓库再 clone 自己的 fork —— 通过 `git commit / push` 把练习进度保存到自己的仓库里。

---

</details>

**第三步 - 开始练习**

```bash
d2x checker                  # 练习循环: 编辑 -> 保存 -> 自动检测 -> 推进
d2x status                   # 进度总览
```

> 想单独跑某一章而不经过 d2x? 直接 `mcpp test -p src/cpp11` 同样可用。

**👉 [更多细节...](https://mcpp-community.github.io/d2mcpp/base/chapter_1.html)**

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
