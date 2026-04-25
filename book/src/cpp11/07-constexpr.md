<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./07-constexpr.html
[English]: ../en/cpp11/07-constexpr.html

# 常量表达式 - constexpr

constexpr 是 C++11 引入的关键字, 用于把"原本要等到运行期才能算出的结果"提前到 **编译期** 就完成, 让编译器在生成代码前就拿到确定值, 同时还保留这段代码在运行期被正常调用的能力

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/constexpr) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/07-constexpr.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/07-constexpr-0.cpp) |  |


**为什么引入?**

- 把可以在编译期完成的计算从运行期搬到编译期, 减少运行期开销
- 让编译器有更强的不变量保证 (例如数组大小、模板非类型参数 等强制要求编译期常量的位置, 可以填一段函数计算的结果)
- 配合模板元编程 / static_assert / enum 等场景, 显式表达 "这个值就是个编译期已知量"

**constexpr 和 const 的区别?**

- const: "我不会改它" — 值可能要等到运行期才知道, 只要求一旦初始化就不可修改
- constexpr: "这个值在编译期就确定" — 是更强的约束, 由编译器保证可在编译期求值
- 所有 constexpr 变量都是 const, 但不是所有 const 都能放到 `constexpr` 要求的位置 (例如数组维度、模板非类型参数)

## 一、基础用法和场景

### constexpr 变量 — 必须用编译期常量初始化

constexpr 变量的初始化表达式必须能被编译器在编译期求出, 否则直接编译报错. 它的核心定位是"编译期常量"

```cpp
int n = 10;
const int a = n + 10;     // ok: a 是 const, 但值是运行期才确定的
constexpr int b = 10 * 3; // ok: b 在编译期就是 30
// constexpr int c = n;   // 错误: n 是运行期变量, 不能用来初始化 constexpr 变量
```

`const` 只承诺"不再修改", 而 `constexpr` 进一步要求"现在就能算出"

### 编译期常量 vs 运行期常量 — 用在数组维度上的差异

C++ 中数组维度要求是编译期常量. 在这个位置, 普通 `int` 和 "值由运行期变量推导出的 const" 都不能用, 只有 constexpr 才稳

```cpp
int size1 = 10;
const int size2 = size1 + 10;
constexpr int size3 = 10 * 3;

int arr1[size3]; // ok: size3 是编译期常量
// int arr2[size1]; // 错误: size1 是运行期变量
// int arr3[size2]; // 取决于编译器: size2 由 size1 推导, 不一定是编译期常量
```

练习 0 中需要在 `arr1[sizex]` 里挑出唯一一个能稳定保证编译期已知的维度, 答案就是 `size3`

### constexpr 函数 — 既能编译期用, 也能运行期用

constexpr 函数最大的特点是 **双形态**: 给它编译期常量参数, 它就在编译期算; 给它运行期变量参数, 它就退化成普通函数在运行期算

```cpp
constexpr int sum_for_1_to(int n) {
    return n == 1 ? 1 : n + sum_for_1_to(n - 1);
}

int main() {
    constexpr int s1 = sum_for_1_to(4); // 编译期算出 10
    int n = 5;
    int s2 = sum_for_1_to(n);           // 运行期算
}
```

注意: 一个函数即使加了 constexpr, 也不强制它每次都在编译期被调用 — 是否走编译期, 取决于 **使用位置** 和 **传入的参数**

### 在需要"必须编译期常量"的位置使用 constexpr 函数

数组维度、模板非类型参数、`static_assert`、`case` 标签 — 这些位置都要求编译期常量. 把计算逻辑封装进 constexpr 函数, 就能在这些位置直接调用

```cpp
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fact_10 = factorial(10);
int arr[factorial(5)];                  // 数组维度: ok
static_assert(factorial(5) == 120, ""); // 静态断言: ok
```

### 配合模板做编译期计算

模板的非类型参数 (`template <int N>`) 也要求编译期常量, constexpr 函数 / constexpr 变量都能填进去

```cpp
template <int N>
struct Sum {
    static constexpr int value = Sum<N - 1>::value + N;
};

template <>
struct Sum<1> { static constexpr int value = 1; };

constexpr int sum_4 = Sum<4>::value; // 编译期得到 10
```

把 `factorial` 和 `Sum` 组合起来还能在编译期解一些小问题 — 例如练习 1 里 "value 取多少时 `value! + (1+2+..+value) > 10000`" 这个问题, 全程不需要运行期参与

```cpp
constexpr int value = 8;
constexpr int f = factorial(value);
constexpr int s = Sum<value>::value;
constexpr int ans = f + s;
static_assert(ans > 10000, "ans should be > 10000");
```

### C++11 的 constexpr 函数限制

C++11 对 constexpr 函数的函数体限制比较严格:

- 函数体本质上只能是 **一条 return 语句** (可以用三目 `?:` 替代分支)
- 不能写循环 (要靠递归代替)
- 不能有局部变量定义、不能改值

```cpp
// ok: C++11 风格的 constexpr 函数 — 单 return + 三目 + 递归
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}
```

C++14 起放宽了这些限制, 允许局部变量、循环、多语句, 写起来跟普通函数几乎一样. 但作为 C++11 章节的内容, 这里仍以 C++11 的"单 return + 递归"为基本写法

## 二、注意事项

### constexpr 函数被传"运行期参数"时, 它就是普通运行期函数 — 不会报错

constexpr 是一种 **能力声明**, 不是 **使用要求**. 同一个 constexpr 函数, 在 constexpr 变量初始化处会被强制走编译期, 在普通赋值处则按运行期函数执行

```cpp
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

int n = 5;
int a = factorial(n);           // ok: 运行期调用 (n 是运行期变量)
constexpr int b = factorial(5); // ok: 编译期调用
// constexpr int c = factorial(n); // 错误: 编译期上下文需要常量, 但 n 不是
```

### 函数内部调用的 constexpr 函数, 也必须是 constexpr

如果一个 constexpr 函数内部调用了非 constexpr 函数, 那它在编译期上下文里就用不了 — 编译器会拒绝把整段表达式当成常量表达式

```cpp
double pow(double base, int exp) {              // 非 constexpr
    return exp == 0 ? 1.0 : base * pow(base, exp - 1);
}

constexpr double mysin(double x) {
    return x - pow(x, 3) / 6.0; // 错误: 用了非 constexpr 的 pow
}
```

把 `pow` 改成 `constexpr double pow(...)` 后, `mysin` 才能真正在编译期算出值. 练习 1 里的 `mysin(30.0)` 就是这种"打表"风格 — 修好后整个 sin 值由编译器算完直接写进二进制, 运行期复杂度 O(1)

### 不要为了 constexpr 而 constexpr

constexpr 把计算搬到编译期, 也意味着错误诊断、调试栈信息全要在编译期处理, 一旦逻辑变复杂, 编译错误信息会非常吓人. 经验法则:

- 真正需要 "在数组维度 / 模板参数 / static_assert 里直接用上" 的计算 → 用 constexpr
- 普通的工具函数, 即使能写成 constexpr 也不必非加 — 让编译期 / 运行期成本更可控

### constexpr 不等于 inline 也不等于 noexcept

但有一个细节常被忽略: **constexpr 函数是隐式 inline 的**. 这意味着多个翻译单元里包含同一个 constexpr 函数定义不会触发 ODR 冲突, 头文件里直接定义即可, 不需要再额外加 `inline`

## 三、练习代码

### 练习代码主题

- 0 - [编译期计算基础: constexpr 和 const 的区别](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/07-constexpr-0.cpp)
- 1 - [编译期计算应用示例: factorial / Sum / mysin 自动打表](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/07-constexpr-1.cpp)

### 练习代码自动检测命令

```
d2x checker constexpr
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
