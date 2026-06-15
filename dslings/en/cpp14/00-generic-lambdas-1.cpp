// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp14/00-generic-lambdas-1.cpp
//
// Exercise: cpp14 | 00 - generic lambdas | generic lambda with STL algorithms
//
// Tips:
// - A single generic lambda can be reused with containers of different
//   element types
// - Captured variables retain their concrete types; only parameters use auto
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/lambda
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/00-generic-lambdas.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker generic-lambdas
//

#include <d2x/cpp/common.hpp>
#include <vector>
#include <algorithm>

int main() {

    // 0. Same generic lambda used to sort containers of different element types
    std::vector<int> v1 = {5, 1, 4, 2, 8};
    std::vector<double> v2 = {3.1, 2.7, 8.5, 1.9};

    auto desc = [](D2X_YOUR_ANSWER a, D2X_YOUR_ANSWER b) {
        return a > b;
    };

    std::sort(v1.begin(), v1.end(), desc);
    d2x_assert_eq(v1[0], 8);
    d2x_assert_eq(v1[4], 1);

    std::sort(v2.begin(), v2.end(), desc);
    d2x_assert_eq(v2[0], D2X_YOUR_ANSWER);

    // 1. Generic lambda with capture — find_if
    int threshold = 3;
    auto above = [threshold](auto x) {
        return x > threshold;
    };

    auto it1 = std::find_if(v1.begin(), v1.end(), above);
    d2x_assert(*it1 == D2X_YOUR_ANSWER);

    auto it2 = std::find_if(v2.begin(), v2.end(), above);
    d2x_assert(*it2 == 8.5);

    // 2. Generic lambda returning lambda — factory function
    auto make_multiplier = [](auto factor) {
        return [factor](auto x) { return x * D2X_YOUR_ANSWER; };
    };

    auto times2 = make_multiplier(2);
    d2x_assert_eq(times2(10), 20);
    d2x_assert_eq(times2(0.5), 1.0);

    D2X_WAIT

    return 0;
}
