<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./12-nullptr.html
[English]: ../en/cpp11/12-nullptr.html

# nullptr - 指针字面量

`nullptr` 是C++11引入的**指针字面量**，用于表示空指针。它解决了传统空指针表示方式（如`NULL`和`0`）在类型安全性和重载解析方面的不足。

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/nullptr) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/12-nullptr.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/12-nullptr-0.cpp) |  |

**为什么引入?**

- 解决`NULL`宏和整数`0`在重载解析中的歧义问题
- 提供类型安全的空指针表示方式
- 明确区分指针和整数类型
- 支持模板编程中的类型推导

**nullptr和NULL有什么区别?**

- `nullptr`是C++11引入的关键字，类型为`std::nullptr_t`
- `NULL`是预处理宏，通常定义为整数`0`或`(void*)0`
- `nullptr`在重载解析中更精确，不会与整数类型混淆

## 一、基础用法和场景

### 替代NULL和0

> 用于指针变量的初始化和赋值，替代传统的`NULL`和`0`

```cpp
int* ptr1 = nullptr;        // 推荐用法
int* ptr2 = NULL;           // 传统用法
int* ptr3 = 0;              // 不推荐

// 检查指针是否为空
if (ptr1 == nullptr) {
    // 处理空指针情况
}
```

### 解决重载歧义问题

> 在函数调用中明确传递空指针，`nulltpr`能避免重载歧义问题, 并且避免与整数类型的混淆

```cpp
void func(int* ptr) {
    if (ptr != nullptr) {
        *ptr = 42;
    }
}

void func(int value) {
    // 处理整数参数
}

int main() {
    func(nullptr);  // 明确调用指针版本
    func(0);        // 可能调用整数版本，产生歧义
    func(NULL);     // 可能调用整数版本，产生歧义
}
```

例如上面的代码中,调用`func(NULL)`就会报重载歧义错误

```bash
main.cpp: In function 'int main()':
main.cpp:16:9: error: call of overloaded 'func(NULL)' is ambiguous
   16 |     func(NULL);     // 可能调用整数版本，产生歧义
      |     ~~~~^~~~~~
```

### 确保模板编程中的类型安全

> 在模板函数和类中，`nullptr`提供更好的类型推导和安全性

```cpp
// https://en.cppreference.com/w/cpp/language/nullptr.html

template<class T>
constexpr T clone(const T& t) {
    return t;
}

void g(int*) {
    std::cout << "Function g called\n";
}

int main() {
    g(nullptr);        // ok
    g(NULL);           // ok
    g(0);              // ok

    g(clone(nullptr)); // ok
    g(clone(NULL));    // ERROR: NULL可能会被推导成非"指针"类型
    g(clone(0));       // ERROR: 0会被推导成非"指针"类型
}
```

当使用函数模板时, `NULL`和`0`通过会被推导成非"指针"类型, 而`nullptr`可以避免这个问题

```bash
main.cpp:19:12: error: invalid conversion from 'int' to 'int*' [-fpermissive]
   19 |     g(clone(0));       // ERROR: 0会被推导成非"指针"类型
      |       ~~~~~^~~
      |            |
      |            int
```

### 智能指针和容器

> 与现代C++特性（如智能指针、STL容器）配合使用

```cpp
#include <memory>
#include <vector>

int main() {
    std::shared_ptr<int> sp1 = nullptr;
    std::unique_ptr<int> up1 = nullptr;

    std::vector<int*> vec;
    vec.push_back(nullptr);

    // 检查智能指针是否为空
    if (sp1 == nullptr) {
        sp1 = std::make_shared<int>(42);
    }
}
```

## 二、注意事项

### 类型推导和std::nullptr_t

`nullptr`的类型是`std::nullptr_t`，这是一个特殊的类型，可以 **隐式** 转换为任何指针类型：

```cpp
#include <cstddef>  // 包含std::nullptr_t的定义

void func(int*) {}
void func(double*) {}
void func(std::nullptr_t) {}

int main() {
    auto ptr = nullptr;  // ptr的类型是std::nullptr_t

    func(nullptr);       // 调用std::nullptr_t版本
    func(ptr);           // 调用std::nullptr_t版本

    int* intPtr = nullptr;
    func(intPtr);        // 调用int*版本
}
```

### 与布尔类型的隐式转换

`nullptr`可以隐式转换为`bool`类型，在条件判断中非常方便：

```cpp
int* ptr = nullptr;

if (ptr) { // 等价于 if (ptr != nullptr)
    // 指针非空
} else {
    // 指针为空
}

bool isEmpty = (ptr == nullptr);  // true
```

## 三、练习代码

### 练习代码主题

- 0 - [nullptr基础用法](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/12-nullptr-0.cpp)
- 1 - [nullptr的函数重载](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/12-nullptr-1.cpp)
- 2 - [nullptr在模板编程中的优势](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/12-nullptr-2.cpp)

### 练习代码自动检测命令

```bash
d2x checker nullptr
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://xlings.d2learn.org)
