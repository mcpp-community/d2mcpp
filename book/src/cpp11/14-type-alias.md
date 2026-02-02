<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./14-type-alias.html
[English]: ../en/cpp11/14-type-alias.html

# 类型别名和别名模板

类型别名和别名模板是C++11引入的重要特性，用于为现有类型创建新的名称，增强泛型编程的表达能力，提高代码的可读性和可维护性。

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-type-alias](https://en.cppreference.com/w/cpp/language/type_alias) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/14-type-alias.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/14-type-alias-0.cpp) |  |

> 注: `using`关键字在C++11之前就已经存在, 但当时主要是作为命名空间和类成员声明来使用的
> - 声明命名空间: `using namespace std;`
> - 类成员声明: `struct B : A { using A::member; };`

**为什么引入?**

- 替代传统的`typedef`语法，提供更直观的类型别名定义方式
- 支持模板别名，增强泛型编程的表达能力
- 提高代码可读性，特别是对于复杂类型
- 与`using`声明语法保持一致

**类型别名和typedef有什么区别?**

- 语法更直观：`using NewType = OldType;` vs `typedef OldType NewType;`
- 支持模板别名，而`typedef`不支持
- 在模板编程中更加灵活和强大

## 一、基础用法和场景

### 基本类型别名

> 为现有类型创建新的名称，提高代码可读性, 并且可以取代传统`typedef`定义别名的方式

```cpp
typedef int Integer; // 传统typedef方式
using Integer = int; // C++11 using方式

// 使用别名
Integer i = 1;
int j = 2;
```

类型别名并不是一个新的类型, 而是其他复合类型的一个别名, 本质是一样的。像上的代码中`Integer`的本质就是`int`， 常用于简化类型名

### 复杂类型别名

> 为复杂类型（如函数指针、嵌套类型）创建别名

```cpp
// 函数指针别名
using FuncPtr = void(*)(int, int);
using StringVector = std::vector<std::string>;

// 嵌套类型别名
struct Container {
    using ValueType = int;
    using Iterator = std::vector<ValueType>::iterator;
};

void example(int a, int b) {
    // 函数实现
}

int main() {
    FuncPtr func = example; // 等价: void(*func)(int, int) = example;
    StringVector strings = {"hello", "world"}; // 等价: std::vector<std::string> strings...
    Container::ValueType value = 100; // 等价: int value = 100;
    return 0;
}
```

对于`void  (*func) (int, int) = example;`这样的代码很多人看到可能都要迟疑一下才能反应过来, 它是定义了一个函数指针。通过使用`using`给个复杂类型起一个类型别名`FuncPtr`， 使用`FuncPtr func = example;`就能让人快速获取代码意图了

### 别名模板

> 为模板类型创建别名，增强泛型编程能力

```cpp
// 别名模板
template <typename T>
using Vec = std::vector<T>;

// 基于泛型, 创建其"子集合"别名类型
template <typename T>
using Vec3 = std:Array<T, 3>;
template <typename T>
using Vec4 = std:Array<T, 4>;

// 带默认参数的别名模板
template <typename T, typename Compare = std::less<T>>
using Heap = std::priority_queue<T, std::vector<T>, Compare>;

int main() {
    Vec<int> numbers = {1, 2, 3};
    Vec3<float> v3 = {1.0f, 2.0f, 3.0f};
    Vec4<float> v4 = {1.0f, 2.0f, 3.0f, 4.0f};
    Heap<int> minHeap;
    Heap<int, std::greater<int>> maxHeap;
    return 0;
}
```

除了给复杂创建别名外, 还支持给模板类型创建别名, 并且通过模板参数还能实现对原模板类型的参数/属性进行控制 - 默认参数、分配器类型、长度、比较器等. 在上面的代码中我们分别创建了动态Vec类型别名; 也通过指定长度, 创建了定长的`Vec3`、`Vec4`服务于特殊场景(向量、矩阵计算)的类型别名; 还用模板参数默认指创建了`Heap`类型, 底层默认使用`vector`作为数据结构, 并支持默认最小堆, 和通过指定模板参数的方式设置最大堆

### 标准库中_t风格的模板

> 在STL中有一些模板, 会提供_t的版本, 来节省手动获取类型和取值的过程。使用类型别名可以轻松的实现他们(_v风格需要C++17的[inline variables + variable templates]的支持)

`std:remove_const_t的参考实现`

```cpp
// remove_const的实现和原理解释可参考: https://zhuanlan.zhihu.com/p/352972564
template <typename T>
using my_remove_const_t = typename std::remove_const<T>::type;

int main() {
    const int a = 10;
    my_remove_const_t<decltype(a)> b = a; // b的类型为int，而不是const int
    return 0;
}
```

## 二、注意事项

### 别名不是新类型

> 类型别名只是现有类型的同义词，不会创建新类型

```cpp
using MyInt = int;
using YourInt = int;

int main() {
    MyInt a = 10;
    YourInt b = 20;

    a = b;  // 可以赋值，因为都是int类型
    static_assert(std::is_same<MyInt, YourInt>::value, "Types are the same");

    return 0;
}
```

### 模板别名的作用域

> 别名模板必须在类作用域或命名空间作用域中声明

```cpp
namespace MyNamespace {
    template<typename T>
    using MyVector = std::vector<T>;
}

class MyClass {
public:
    template<typename T>
    using Ptr = T*;
};

// 错误：不能在函数作用域中声明别名模板
// void func() {
//     template<typename T>
//     using LocalAlias = T;  // 编译错误
// }
```

### 递归别名限制

> 别名模板不能直接或间接引用自身

```cpp
template<typename T>
struct A;

// 错误：递归别名
// template<typename T>
// using B = typename A<T>::U;

template<typename T>
struct A {
    // typedef B<T> U;  // 这会导致递归定义错误
};
```

## 三、练习代码

### 练习代码主题

- 0 - [基本类型别名](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/14-type-alias-0.cpp)
- 1 - [复杂类型和函数指针别名](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/14-type-alias-1.cpp)
- 2 - [别名模板基础](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/14-type-alias-2.cpp)
- 3 - [标准库中的别名模板应用](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/14-type-alias-3.cpp)

### 练习代码自动检测命令

```bash
d2x checker type-alias
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
