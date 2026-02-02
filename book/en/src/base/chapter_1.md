<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../base/chapter_1.html
[English]: ./chapter_1.html

# Usage Guide

**d2mcpp** is a hands-on tutorial project focused on Modern C++ core language features. Based on the [xlings(d2x) tool](https://xlings.d2learn.org), it implements a **compiler-driven development model** for code practice that can automatically detect exercise code status and navigate to the next exercise.

## 0. xlings Tool Installation

> xlings contains the tools required for the tutorial project - [More tool details](https://xlings.d2learn.org)

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

## 1. Get Project and Auto-configure Environment

> Download the project to current directory and automatically configure local environment

```bash
d2x install d2mcpp
```

### Local E-book

> Execute `d2x book` command in the project directory to open local documentation (includes usage guide and e-book)

```bash
d2x book
```

### Practice Code Auto-detection

> Enter the project directory `d2mcpp` and run the checker command to enter the practice code auto-detection program

```bash
d2x checker
```

### Specify Exercise for Detection

```bash
d2x checker [name]
```

> Note: Exercise names support fuzzy matching

### Sync Latest Practice Code

> Since the project is continuously updated, you can use the following command for automatic synchronization (if synchronization fails, you may need to manually update the project code using git)

```bash
d2x update
```

## 2. Automated Detection Program Introduction

After entering the automated code practice environment using `xlings checker`, the tool will automatically locate and open the corresponding practice code file, and output compiler errors and hints in the console. The detection program generally has two detection phases: the first is compile-time detection, where you need to fix compilation errors based on hints in the practice code and compiler error messages in the console; the second is runtime detection, which checks if the current code passes all checkpoints when running. When compilation errors are fixed and all checkpoints are passed, the console will display that the current exercise is completed and prompt you to proceed to the next exercise.

**Practice Code File Example**

```cpp
// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/hello-mcpp.cpp
//
// Exercise: Automated Code Practice Tutorial
//
// Tips:
//    This project uses the xlings tool to build automated code practice projects. Execute
//    xlings checker in the project root directory to enter "compiler-driven development mode"
//    for automatic exercise code detection.
//    You need to modify errors in the code based on console error messages and hints.
//    When all compilation errors and runtime checkpoints are fixed, you can delete or comment
//    out the D2X_WAIT macro in the code to automatically proceed to the next exercise.
//
//      - D2X_WAIT: This macro isolates different exercises. You can delete or comment it out to proceed to the next exercise.
//      - d2x_assert_eq: This macro is used for runtime checkpoints. You need to fix code errors so that all
//      - D2X_YOUR_ANSWER: This macro indicates code that needs modification, typically used for code completion (replace this macro with correct code)
//
// Auto-Checker Command:
//
//   d2x checker hello-mcpp
//

#include <d2x/cpp/common.hpp>

// You can observe "real-time" changes in the console when modifying code

int main() {

    std::cout << "hello, mcpp!" << std:endl; // 0. Fix this compilation error

    int a = 1.1; // 1. Fix this runtime error, change int to double to pass the check

    d2x_assert_eq(a, 1.1); // 2. Runtime checkpoint, need to fix code to pass all checkpoints (cannot directly delete checkpoint code)

    D2X_YOUR_ANSWER b = a; // 3. Fix this compilation error, give b an appropriate type

    d2x_assert_eq(b, 1); // 4. Runtime checkpoint 2

    D2X_WAIT // 5. Delete or comment out this macro to proceed to the next exercise (project formal code practice)

    return 0;
}
```

**Console Output and Explanation**

```bash
🌏Progress: [>----------] 0/10 -->> Shows current exercise progress

[Target: 00-0-hello-mcpp] - normal -->> Current exercise name

❌ Error: Compilation/Running failed for dslings/hello-mcpp.cpp -->> Shows detection status

 The code exist some error!

---------C-Output--------- - Compiler output information
[HONLY LOGW]: main: dslings/hello-mcpp.cpp:24 - ❌ | a == 1.1 (1 == 1.100000) -->> Error hint and location (line 24)
[HONLY LOGW]: main: dslings/hello-mcpp.cpp:26 - 🥳 Delete the D2X_WAIT to continue...


AI-Tips-Config: https://d2learn.org/docs/xlings -->> AI hints (requires configuring large model key, optional)

---------E-Files---------
dslings/hello-mcpp.cpp -->> Current detected file
-------------------------

Homepage: https://github.com/d2learn/xlings
```

## 3. Configure Project (Optional)

### Configure Language

Edit the `lang` attribute in the project configuration file `.d2x.json`. `zh` corresponds to Chinese, and `en` corresponds to English.

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

### Custom Editor - Using nvim as Example

If you prefer to use Neovim as your editor with LSP (clangd) support, you can configure it as follows:

**1. Edit the `editor` attribute in the project configuration file `config.xlings` and set it to `nvim` (or `zed`)**

```bash
d2x = {
    checker = {
        name = "dslings",
        editor = "nvim", -- option: vscode, nvim, zed
    },
```

**2. Run the one-click dependency installation and environment configuration command in the project root directory**

```bash
xlings install
```

**3. In the project directory, rerun the detection command `d2x checker` to open the corresponding exercise file with Neovim, which will support automatic exercise navigation/switching**

> Note: In Neovim, the "real-time detection feature" is triggered by the `:w` command. That is, after modifying the code, saving the file in Neovim's command-line mode (`:w`) will prompt `d2x` to update the detection results.


## 4. Resources and Communication

**Communication Group (Q):** 167535744

**Tutorial Discussion Section:** [https://forum.d2learn.org/category/20](https://forum.d2learn.org/category/20)

**xlings:** [https://github.com/d2learn/xlings](https://github.com/d2learn/xlings)

**Tutorial Repository:** [https://github.com/mcpp-community/d2mcpp](https://github.com/mcpp-community/d2mcpp)

**Tutorial Video Collection:** [https://space.bilibili.com/65858958/lists/5208246](https://space.bilibili.com/65858958/lists/5208246)
