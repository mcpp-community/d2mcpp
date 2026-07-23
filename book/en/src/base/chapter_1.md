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
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash
```

or

```bash
wget https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh -O - | bash
```

**Windows - PowerShell**

```bash
Invoke-Expression (Invoke-Webrequest 'https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.ps1' -UseBasicParsing).Content
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

### View Learning Progress

> Read-only progress overview grouped by chapter, without entering practice mode

```bash
d2x status
```

### Native Mode (optional)

> Every C++ standard directory is a real mcpp project and the exercises are its tests/ — you can practice with plain mcpp, no d2x involved:

```bash
mcpp test -p src/en/cpp11              # progress table for the whole cpp11 section
mcpp test -p src/en/cpp11 03-trailing  # run only exercises matching the name
```

### Sync Latest Practice Code

> The project is continuously updated; update via git (save/commit your modified exercises first):

```bash
git pull
```

## 2. Automated Detection Program Introduction

After entering the automated code practice environment using `d2x checker`, the tool will automatically locate and open the corresponding practice code file, and output compiler errors and hints in the console. The detection program generally has two detection phases: the first is compile-time detection, where you need to fix compilation errors based on hints in the practice code and compiler error messages in the console; the second is runtime detection, which checks if the current code passes all checkpoints when running. When compilation errors are fixed and all checkpoints are passed, the console will display that the current exercise is completed and prompt you to proceed to the next exercise.

**Practice Code File Example**

```cpp
// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/intro/tests/hello-mcpp.cpp
//
// Exercise: Automated Code Practice Tutorial
//
// Tips:
//    This is an "exercises are tests" automated practice project. Two modes of use:
//
//      d2x checker                  guided mode: auto-detects your edits, passing advances you
//      mcpp test -p src/en/intro    native mode: plain mcpp — the test report IS your progress
//
//    Fix the code based on console error messages. There are only three conventions:
//
//      - D2X_YOUR_ANSWER: the fill-in-the-blank placeholder — replace it with correct
//        code. It is not a macro, just a name nobody defines, so the compiler error
//        points exactly at the blank
//      - d2x::check / d2x::check_eq: runtime checkpoints — fix the code so every
//        check passes (do not delete the checkpoints)
//      - d2x::wait(): the barrier between exercises — delete it once you have read
//        the lesson to really finish it
//
// Auto-Checker Command:
//
//   d2x checker hello-mcpp
//

import std;
import d2x;

// You can observe "real-time" changes in the console when modifying code

int main() {

    std::cout << "hello, mcpp!" << std:endl; // 0. Fix this compilation error

    int a = 1.1; // 1. Fix this runtime error, change int to double to pass the check

    d2x::check_eq(a, 1.1, "a == 1.1"); // 2. Runtime checkpoint, need to fix code to pass all checkpoints (cannot directly delete checkpoint code)

    D2X_YOUR_ANSWER b = a; // 3. Fix this compilation error, give b an appropriate type

    d2x::check_eq(b, 1, "b == 1"); // 4. Runtime checkpoint 2

    d2x::wait(); // 5. Delete or comment out this line to proceed to the next exercise (project formal code practice)

    return 0;
}
```

**Console Output and Explanation**

```bash
🌏Progress: [>----------] 0/52 -->> Shows current exercise progress

[Exercise: hello-mcpp] -->> Current exercise name

❌ Error: Compilation/Running failed for src/en/intro/tests/hello-mcpp.cpp -->> Shows detection status

 The code exist some error!

---------Output--------- - Compile/run output information
❌ | a == 1.1 (1 == 1.1)  --> src/en/intro/tests/hello-mcpp.cpp:46 -->> Error hint and location (line 46)
🚧 | Delete the d2x::wait() to continue  --> src/en/intro/tests/hello-mcpp.cpp:52


AI-Tips-Config: https://xlings.d2learn.org/en/documents/d2x/intro.html -->> AI hints (requires configuring large model key, optional)

---------E-Files---------
src/intro/tests/hello-mcpp.cpp -->> Current detected file
-------------------------

Homepage: https://github.com/openxlings/xlings
```

## 3. Configure Project (Optional)

### Configure Language

Edit the `lang` attribute in the project configuration file `.d2x.json`. `zh` corresponds to Chinese, and `en` corresponds to English.

```bash
{
    "version": "0.1.1",
    "buildtools": "mcpp run -q -p d2x/buildtools --",
    "lang": "en",
    ...
}
```

### Custom Editor - Using nvim as Example

If you prefer to use Neovim as your editor with LSP (clangd) support, you can configure it as follows:

**1. Edit the `editor` field in the project configuration file `.d2x.json` and set it to `nvim` (or `zed`)**

```json
{
    "buildtools": "mcpp run -q -p d2x/buildtools --",
    "editor": "nvim",
    ...
}
```

> When unset, d2x falls back to `$VISUAL` → `$EDITOR` → `code`; a `{file}` placeholder is supported; an explicit empty string disables auto-open.

**2. Run the one-click dependency installation and environment configuration command in the project root directory**

```bash
xlings install
```

**3. In the project directory, rerun the detection command `d2x checker` to open the corresponding exercise file with Neovim, which will support automatic exercise navigation/switching**

> Note: In Neovim, the "real-time detection feature" is triggered by the `:w` command. That is, after modifying the code, saving the file in Neovim's command-line mode (`:w`) will prompt `d2x` to update the detection results.


## 4. Resources and Communication

**Communication Group (Q):** 167535744

**Tutorial Discussion Section:** [https://forum.d2learn.org/category/20](https://forum.d2learn.org/category/20)

**xlings:** [https://github.com/openxlings/xlings](https://github.com/openxlings/xlings)

**Tutorial Repository:** [https://github.com/mcpp-community/d2mcpp](https://github.com/mcpp-community/d2mcpp)

**Tutorial Video Collection:** [https://space.bilibili.com/65858958/lists/5208246](https://space.bilibili.com/65858958/lists/5208246)
