// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/hello-mcpp.cpp
//
// Exercise: Automated Code Practice Usage Tutorial
//
// Tips:
//    This project is an automated code practice project built using the xlings tool. 
//    By executing `xlings checker` in the project root directory, you enter the 
//    "compiler-driven development mode" for automatic code practice detection.
//    You need to modify errors in the code based on console error messages and prompts.
//    After fixing all compilation errors and runtime checkpoints, you can delete or 
//    comment out the D2X_WAIT macro in the code to automatically proceed to the next exercise.
//
//      - D2X_WAIT: This macro is used to isolate different exercises. You can delete or 
//        comment out this macro to proceed to the next exercise.
//      - d2x_assert_eq: This macro is used for runtime checkpoints. You need to fix 
//        errors in the code so that all checkpoints pass.
//      - D2X_YOUR_ANSWER: This macro indicates code that needs to be modified, generally 
//        used for code completion (replace this macro with correct code)
//
// Docs:
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/chapter_1.md
//   - book/src/chapter_1.md
//
// Auto-Checker command:
//
//   d2x checker hello-mcpp
//

#include <d2x/common.hpp>

// You can observe "real-time" changes in the console when modifying code

int main() {

    std::cout << "hello, mcpp!" << std::endl; // 0. Fix this compilation error

    int a = 1.1; // 1. Fix this runtime error, change int to double to pass the check

    d2x_assert_eq(a, 1.1); // 2. Runtime checkpoint, need to fix code to pass all checkpoints (cannot directly delete checkpoint code)

    int b = a; // 3. Fix this compilation error, give b an appropriate type

    d2x_assert_eq(b, 1); // 4. Runtime checkpoint 2

    D2X_WAIT // 5. Delete or comment out this macro to proceed to the next exercise (project formal code practice)

    return 0;
}

//// --- More detailed introduction | | |
//                  V V V
/*

# [[ Console Output Interpretation ]]

🌏Progress: [>----------] 0/10 -->> Shows current exercise progress

[Target: 00-0-hello-mcpp] - normal -->> Current exercise name

❌ Error: Compilation/Running failed for dslings/hello-mcpp.cpp -->> Shows detection status

 The code exist some error!

---------C-Output--------- - Compiler output information
[HONLY LOGW]: main: dslings/hello-mcpp.cpp:24 - ❌ | a == 1.1 (1 == 1.100000) -->> Error prompt and location (line 24)
[HONLY LOGW]: main: dslings/hello-mcpp.cpp:26 - 🥳 Delete the D2X_WAIT to continue...


AI-Tips-Config: https://d2learn.org/docs/xlings -->> AI tips (requires configuring large model key, optional)

---------E-Files---------
dslings/hello-mcpp.cpp -->> Current file being checked
-------------------------

Homepage: https://github.com/d2learn/xlings

*/