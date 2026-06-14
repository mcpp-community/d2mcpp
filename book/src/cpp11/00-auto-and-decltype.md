<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./00-auto-and-decltype.html
[English]: ../en/cpp11/00-auto-and-decltype.html

# 类型自动推导 - auto和decltype

auto 和 decltype 是C++11引入的强有力的**类型自动推导**工具. 不仅让代码变得更加简洁, 还增强了模板和泛型的表达能力

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-auto](https://en.cppreference.com/w/cpp/language/auto) / [cppreference-decltype](https://en.cppreference.com/w/cpp/language/decltype) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/00-auto-and-decltype.md) | [视频解读](https://www.bilibili.com/video/BV1EzJs6HEf7) / [练习讲解](https://www.bilibili.com/video/BV1xkdYYUEyH) | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-0.cpp) |  |


**为什么引入?**

- 解决类型声明过于复杂的问题
- 模板应用中, 获取对象或表达式类型的需求
- 为lambda表达式的定义做支撑

**auto和decltype有什么区别?**

- auto常常用于变量定义, 推导的类型可能丢失const或引用(可显式指定进行保留auto &)
- decltype获取表达式的**精确类型**
- auto通常无法作为模板类型参数使用

## 一、基础用法和场景

### 声明定义

> 充当类型占位符, 辅助变量的定义或声明。使用auto时变量必须要初始化, decltype可以不用初始化

```cpp
int b = 2;
auto b1 = b;
decltype(b) b2 = b;
decltype(b) b3; // 可以不用初始化
```

### 表达式类型推导

> 常常用于复杂表达式的类型推导, 确保计算精度

```cpp
int a = 1;

auto b1 = a + 2;
decltype(a + 2 + 1.1) b2 = a + 2 + 1.1;

auto c1 = a + '0';
decltype(2 + 'a') c2 = 2 + 'a';
```

### 复杂类型推导

**迭代器类型推导**

```cpp
std::vector<int> v = {1, 2, 3};

// std::vector<int>::iterator it = v.begin();
auto it = v.begin(); // 自动推导it类型
// decltype(v.begin()) it = v.begin();
for (; it != v.end(); ++it) {
    if (*it == 2) {
        v.insert(it, 0);
        break;
    }
}
```

**函数类型推导**

> 对于函数或lambda表达式这种复杂类型, 常常使用auto和decltype. 一般, lambda定义用auto,  模板类型参数用decltype

```cpp
int add_func(int a, int b) {
    return a + b;
}

int main() {
    auto minus_func = [](int a, int b) { return a - b; };

    std::vector<std::function<decltype(add_func)>> funcVec = {
        add_func,
        minus_func
    };

    funcVec[0](1, 2);
    funcVec[1](1, 2);
    //...
}
```

### 函数返回值类型推导

**语法糖用法**

> auto为后置返回类型函数定义写法做支持, 并可以配合decltype进行返回类型推导使用

```cpp
auto main() -> int {
    return 0;
}

auto add(int a, double b) -> decltype(a + b) {
    return a + b;
}
```

**函数模板返回值类型推导**

> 当无法确定模板返回值时可以用auto + decltype做推导, 可以让add支持一般类型int, double,... 和 复杂类型 Point, Vec,... 增强泛型的表达能力. (c++14中可以省略decltype)

```cpp
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}
```

### 类/结构体成员类型推导

```cpp
struct Object {
    const int a;
    double b;
    Object() : a(1), b(2.0) { }
};

int main() {
    const Object obj;

    auto a = obj.a;
    std::vector<decltype(obj.b)> vec;
}
```

## 二、真实案例 - STL 中的 auto/decltype

> 前述示例用于讲解语法, 而 auto/decltype 的实际价值, 在标准库自身的实现中体现得最为直接。下面从仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 中选取两段常见代码, 分别演示 `auto` 与 `decltype`; `_STD` 等是库内部的宏与限定写法, 阅读时关注 `auto` / `decltype` 即可

### auto 推导迭代器与元素类型:遍历容器

遍历容器是 `auto` 最常见的场景。下面是 `<filesystem>` 规范化路径时的一段遍历:`auto _Pos` 推导出迭代器类型, `const auto _Elem` 推导出解引用后的元素类型 —— 与 `## 一`「复杂类型推导 - 迭代器」里的 `auto it = v.begin()` 完全是同一种写法

```cpp
// MSVC STL · msvc-stl/stl/inc/filesystem (有删节, 缩进保留源码层级)
            auto _New_end = _Vec.begin();
            for (auto _Pos = _Vec.begin(); _Pos != _Vec.end();) {
                const auto _Elem = *_Pos++;
                // ...（根据 _Elem 决定是否写回 _New_end, 略）
            }
```

`_Vec` 是路径分量的容器, 它的迭代器类型与元素类型都交给 `auto` 推导, 无需写出具体类型

### decltype 取变量的类型:std::lower_bound 的二分查找

`decltype` 最直接的用法就是"取某个变量或表达式的类型"。标准库的二分查找 `std::lower_bound` 里, 先用 `auto` 推导出区间长度 `_Count`, 再用 `decltype(_Count)` 表示"与 `_Count` 相同的类型", 把 `_Count / 2` 转换回该类型:

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (有删节) —— std::lower_bound
    auto _UFirst = _STD _Get_unwrapped(_First);
    auto _Count  = _STD distance(_UFirst, _STD _Get_unwrapped(_Last));

    while (0 < _Count) { // divide and conquer, find half that contains answer
        const auto _Count2 = static_cast<decltype(_Count)>(_Count / 2);
        const auto _UMid   = _STD next(_UFirst, _Count2);
        // ...（在 _UMid 处比较, 缩小区间, 略）
    }
```

这正是 `## 一`「声明定义」中 `decltype(b) b2` 的同款用法: `decltype(_Count)` 就是"`_Count` 的类型"。`auto` 负责把类型推导出来, `decltype` 负责在别处复用同一个类型

> 小结: 遍历容器、复用某个变量的类型 —— 这些日常写法在标准库内部用的正是本章这套 auto + decltype 工具。这也是 C++11 引入二者的核心动机之一

## 三、注意事项

### auto 的 const / 引用剥离

> auto 推导会**剥离顶层 const 和引用**, 想保留得显式写 `const auto&` / `auto&`; decltype 则精确保留声明类型

```cpp
int a = 1;
int &b = a;
const int c = 1;
const int &d = c;

auto a1 = a; // int
auto b1 = b; // int
auto c1 = c; // int
auto d1 = d; // int

const auto c2 = c;  // const int
const auto &d2 = d; // const int &

decltype(c) c3 = c; // const int
decltype(d) d3 = d; // const int &
```

这也解释了前面「类/结构体成员类型推导」里 `auto a = obj.a;` 为什么得到的是 `int` 而非 `const int` —— 顶层 const 被 auto 剥离了

### decltype(obj) 和 decltype( (obj) )的区别

- 一般`decltype(obj)`获取的是其声明类型
- 而`decltype( (obj) )` 获取的是 `(obj)` 这个表达式的类型(左值表达式)

```cpp
int a = 1;
decltype(a) b; // 推导结果为a的声明类型int 
decltype( (a) ) c; // 推导结果为(a)这个左值表达式的类型 int & 
```

### decltype(obj.b) 和 decltype( (obj.b) )的区别

- `decltype( (obj.b) )`: 从表达式视角做类型推导, obj定义类型会影响推导结果. 例如, 如果obj被const修饰时, const会限定obj.b的访问为const
- `decltype(obj.b)`: 由于推导的是成员声明类型, 所以不会受obj定义的影响

```cpp
struct Object {
    const int a;
    double b;
    Object() : a(1), b(2.0) { }
};

int main() {
    Object obj;
    const Object obj1;

    decltype(obj.b)  // double
    decltype(obj1.b) // double

    decltype( (obj.b) ) // double & 
    decltype( (obj1.b) ) // 受obj1定义的const修饰影响, 所以是 const double &
}
```

### 右值引用变量, 在表达式中是左值

```cpp
int &&b = 1;

decltype(b) // 推导结果是声明类型 int &&
decltype( (b) ) // 推导结果是 int &
```

## 四、练习代码

### 练习代码主题

- 0 - [声明定义](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-0.cpp)
- 1 - [表达式类型推导](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-1.cpp)
- 2 - [复杂类型推导 - 迭代器 / 函数](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-2.cpp)
- 3 - [函数返回值类型推导](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-3.cpp)
- 4 - [类/结构体成员类型推导](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-4.cpp)
- 5 - [const 与引用的剥离和保留](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-5.cpp)

### 练习代码自动检测命令

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
d2x checker auto-and-decltype
```

### 练习交流讨论

- [auto/decltype 练习讨论帖](https://forum.d2learn.org/post/357)

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [mcpp-community 社区](https://github.com/mcpp-community)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
- [教程检测工具-d2x](https://github.com/d2learn/d2x)