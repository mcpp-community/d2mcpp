<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./chapter_1.html
[English]: ../en/base/chapter_1.html

# 使用说明

**d2mcpp**是一个强调动手实践的现代C++核心语言特性教程项目。基于[xlings(d2x)工具](https://github.com/d2learn/xlings)搭建了一套**编译器驱动开发模式**的代码练习, 可以自动化的检测练习代码的状态和跳转到下一个练习...

## 0.xlings工具安装

> xlings包含教程项目所需的工具 - [更多工具细节](https://github.com/d2learn/xlings)

**Linux**

```bash
curl -fsSL https://d2learn.org/xlings-install.sh | bash
```

or

```bash
wget https://d2learn.org/xlings-install.sh -O - | bash
```

**Windows - PowerShell**

```bash
Invoke-Expression (Invoke-Webrequest 'https://d2learn.org/xlings-install.ps1.txt' -UseBasicParsing).Content
```

## 1.获取项目及自动配置环境

> 下载项目到当前目录并自动配置本地环境

```bash
d2x install d2mcpp
```

### 本地电子书

> 可以在项目目录执行`d2x book`命令, 打开本地文档(包含使用说明和电子书)

```bash
d2x book
```

### 练习代码自动检测

> 进入项目目录`d2mcpp`运行checker命令, 进入练习代码自动检测程序

```bash
d2x checker
```

### 指定练习进行检测

```bash
d2x checker [name]
```

> 注: 练习名支持模糊匹配

### 同步最新的练习代码

> 由于项目处于持续更新阶段, 可以使用下面的命令进行自动同步(如果同步失败, 可能需要手动用git进行更新项目代码)

```bash
d2x update
```

## 2.自动化检测程序简介

使用`d2x checker`进入自动化代码练习环境后, 工具会自动定位打开对应的练习代码文件, 并在控制台输出提示编译器的错误及提示信息。一般检测程序分两个检测阶段: 第一个是编译期检测, 即你需要通过练习代码中的提示信息和控制台编译器的报错, 修复代码的编译错误; 第二个是运行时检测, 即当前代码运行时是否能通过所有检查点。当修复编译错误并通过所有检查点时, 控制台就会显示当前练习通过并提示你进入下一个练习

**代码练习文件示例**

```cpp
// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/hello-mcpp.cpp
//
// Exercise/练习: 自动化代码练习使用教学
//
// Tips/提示:
//    该项目是使用xlings工具搭建的自动化代码练习项目, 通过在项目根目录下
//    执行 xlings checker 进入"编译器驱动开发模式"的练习代码自动检测.
//    你需要根据控制台的报错和提示信息, 修改代码中的错误. 当修复所有编译错误和
//    运行时检查点后, 你可以删除或注释掉代码中的 D2X_WAIT 宏, 会自动进入下一个练习.
//
//      - D2X_WAIT: 该宏用于隔离不同练习, 你可以删除或注释掉该宏, 进入下一个练习.
//      - d2x_assert_eq: 该宏用于运行时检查点, 你需要修复代码中的错误, 使得所有
//      - D2X_YOUR_ANSWER: 该宏用于提示你需要修改的代码, 一般用于代码填空(即用正确的代码替换这个宏)
//
// Auto-Checker/自动检测命令:
//
//   d2x checker hello-mcpp
//

#include <d2x/cpp/common.hpp>

// 修改代码时可以观察到控制台"实时"的变化

int main() {

    std::cout << "hello, mcpp!" << std:endl; // 0.修复这个编译错误

    int a = 1.1; // 1.修复这个运行时错误, 修改int为double, 通过检查

    d2x_assert_eq(a, 1.1); // 2.运行时检查点, 需要修复代码通过所有检查点(不能直接删除检查点代码)

    D2X_YOUR_ANSWER b = a; // 3.修复这个编译错误, 给b一个合适的类型

    d2x_assert_eq(b, 1); // 4.运行时检查点2

    D2X_WAIT // 5.删除或注释掉这个宏, 进入下一个练习(项目正式代码练习)

    return 0;
}
```

**控制台输出及解释**

```bash
🌏Progress: [>----------] 0/10 -->> 显示当前的练习进度

[Target: 00-0-hello-mcpp] - normal -->> 当前的练习名

❌ Error: Compilation/Running failed for dslings/hello-mcpp.cpp -->> 显示检测状态

 The code exist some error!

---------C-Output--------- - 编译器输出信息
[HONLY LOGW]: main: dslings/hello-mcpp.cpp:24 - ❌ | a == 1.1 (1 == 1.100000) -->> 错误提示及位置(24行)
[HONLY LOGW]: main: dslings/hello-mcpp.cpp:26 - 🥳 Delete the D2X_WAIT to continue...


AI-Tips-Config: https://d2learn.org/docs/xlings -->> AI提示(需要配置大模型的key, 可不使用)

---------E-Files---------
dslings/hello-mcpp.cpp -->> 当前检测的文件
-------------------------

Homepage: https://github.com/d2learn/xlings
```

## 3.配置项目(可选)

### 配置语言

编辑项目配置文件`.d2x.json`中的`lang`属性, `zh`对应中文, `en`对应英文

```lua
    },

    private = {
        -- project private attributes
        mcpp = {
            lang = "en", -- option: en, zh
        }
    },
}
```

### 自定义编辑器 - 以nvim编辑器为例

如果你希望使用 Neovim 编辑器并获得 LSP（clangd）支持, 可以按如下步骤进行配置

**1.编辑项目配置文件`config.xlings`中的`editor`属性, 设置为`nvim` (或`zed`)**

```bash
d2x = {
    checker = {
        name = "dslings",
        editor = "nvim", -- option: vscode, nvim, zed
    },
```

**2.在项目根目录运行一键依赖安装和环境配置命令**

```bash
xlings install
```

**3.在项目目录, 重新运行检测命令 `d2x checker` 就会使用nvim打开对应练习文件， 并具备练习自动跳转/切换功能**

> 注: nvim编辑器下的"实时检测功能"的触发时机, 将会对应到 `:w` 命令. 即修改代码后, 在nvim的命令行模式对文件进行保存(`:w`)时, d2x就会更新检测结果

## 4.资源于交流

**交流群(Q):** 167535744

**教程讨论版块:** [https://forum.d2learn.org/category/20](https://forum.d2learn.org/category/20)

**xlings:** [https://github.com/d2learn/xlings](https://github.com/d2learn/xlings)

**教程仓库:** [https://github.com/mcpp-community/d2mcpp](https://github.com/mcpp-community/d2mcpp)

**教程视频合集:** [https://space.bilibili.com/65858958/lists/5208246](https://space.bilibili.com/65858958/lists/5208246)
