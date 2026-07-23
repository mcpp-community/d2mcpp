// mcpp-standard: https://github.com/Sunrisepeak/mcpp-standard
// license: Apache-2.0
// file: src/en/intro/tests/hello-mcpp.cpp
//
// Exercise: Automated Code Practice Usage Tutorial
//
// Tips:
//    This is an "exercises are tests" automated practice project. Two modes of use:
//
//      d2x checker              guided mode: auto-detects your edits, passing advances you
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
// Docs:
//   - https://github.com/Sunrisepeak/mcpp-standard/blob/main/book/src/chapter_1.md
//   - book/src/chapter_1.md
//
// Auto-Checker command:
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

//// --- More detailed introduction | | |
//                  V V V
/*

# [[ Console Output Interpretation ]]

🌏Progress: [>----------] 0/52 -->> Shows current exercise progress

[Exercise: hello-mcpp] -->> Current exercise name

❌ Error: Compilation/Running failed for src/en/intro/tests/hello-mcpp.cpp -->> Shows detection status

 The code exist some error!

---------Output--------- - Compile/run output information
❌ | a == 1.1 (1 == 1.1)  --> src/en/intro/tests/hello-mcpp.cpp:46 -->> Error prompt and location (line 46)
🚧 | Delete the d2x::wait() to continue  --> src/en/intro/tests/hello-mcpp.cpp:52


AI-Tips-Config: https://xlings.d2learn.org/en/documents/d2x/intro.html -->> AI tips (requires configuring large model key, optional)

---------E-Files---------
src/en/intro/tests/hello-mcpp.cpp -->> Current file being checked
-------------------------

Homepage: https://github.com/openxlings/xlings

*/
