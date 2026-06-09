<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./NN-topic.html
[English]: ../en/<std>/NN-topic.html

# 标题 - english name

<一段话简介:该特性由 C++NN 引入,用于解决……,让……>

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/...) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/<std>/NN-topic.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/<std>/NN-topic-0.cpp) |  |


**为什么引入?**

- <动机 1>
- <动机 2>

**和……的区别?**

- <对比点 1>
- <对比点 2>

## 一、基础用法和场景

### <小节标题>

<讲解 + 短代码块,代码内用注释标出"错误写法"与"正确写法">

```cpp
// 示例代码
```

<!-- 每章应有本节;代码必须取自仓库内 msvc-stl/ 并逐字摘录(见 SKILL.md 的
     「真实案例 / msvc-stl」约定)。若该特性在 STL 实现中确无代表性用例,
     可省略本节并在 PR 说明原因。 -->
## 二、真实案例 - STL 中的 <特性>

> 印证该特性在生产级代码中的真实用法。代码取自仓库内置的 MSVC STL 引用基底 [`msvc-stl/`](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl)(源文件 [`stl/inc/<header>`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/<header>));`_NODISCARD` / `_CONSTEXPR17` / `_STD` 等为库内部宏,阅读时可忽略

### <真实用法小标题,呼应「一」中的某个语法点>

<一句话:标准库为什么必须这样用该特性,人手写不出 / 写不优雅>

```cpp
// MSVC STL · stl/inc/<header> (有删节)
// <逐字摘录真实代码;删减处保留 (有删节) 标注,切勿凭记忆改写>
```

<把它和「一」里的玩具示例对应起来:你这样用 → 标准库自己也这样写。这也是该特性被引入的核心动机之一>

## 三、注意事项

### <易错点 / 迁移建议>

<讲解>

<!-- 练习主题列出本章【全部】练习(-0、-1、…,与 dslings/xmake 注册数一致),作为索引;
     但检测命令只放【入口一条】 d2x checker <name> —— d2x 会自动逐个往下推进,无需逐条列出。
     各练习自己的 checker 名 + 针对本练习的 Tips 放在对应练习文件的头注释里。
     讨论帖取各练习头注释里的「练习交流讨论」链接(全章通常同一帖)。 -->
## 四、练习代码

### 练习代码主题

- 0 - [<练习 0 主题>](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/<std>/NN-topic-0.cpp)
- 1 - [<练习 1 主题>](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/<std>/NN-topic-1.cpp)

### 练习代码自动检测命令

<!-- 获取工具命令固定不变,可整段照抄;<details> 放在检测命令【之前】(先给获取方式、再给运行命令);
     <details> 内代码块前后必须留空行才会被 mdBook 渲染 -->
<details>
<summary>还没有 d2x?点击展开获取方式</summary>

```bash
# 1. 安装 xlings(Linux / macOS)
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash
# Windows PowerShell:
# irm https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.ps1 | iex

# 2. 安装 d2x 并拉取本教程
xlings install d2x -y
d2x install d2mcpp

# 3. 进入项目目录 & 运行检查命令
cd d2mcpp
```

</details>

```
d2x checker <name>
```

### 练习交流讨论

- [<本章练习讨论帖>](https://forum.d2learn.org/post/NNN)

<!-- 可选:仅当该特性在后续标准有演进时加入。纯细化/弃用写在这里;
     可独立成章的新能力应在对应标准章节单独成篇并在此前向链接。
     位置:置于 ## 五、其他 之前。 -->
## 特性演进 / Feature Evolution

> **引入标准**:C++NN
> **后续优化**:
> - **C++14**:<...>
> - **C++17**:<...>(详见 [对应章节](../<later-std>/MM-xxx.md))
> - **C++20**:<...>
>
> **教学提示**:<跨标准行为差异 / 弃用 / 本练习为何用例外标准>

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [mcpp-community 社区](https://github.com/mcpp-community)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
- [教程检测工具-d2x](https://github.com/d2learn/d2x)
