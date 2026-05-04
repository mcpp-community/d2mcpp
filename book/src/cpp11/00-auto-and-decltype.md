<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./00-auto-and-decltype.html
[English]: ../en/cpp11/00-auto-and-decltype.html

# 类型自动推导 - auto和decltype

auto 和 decltype 是C++11引入的强有力的**类型自动推导**工具. 不仅让代码变的更加简洁, 还增强了模板和泛型的表达能力

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-auto](https://en.cppreference.com/w/cpp/language/auto) / [cppreference-decltype](https://en.cppreference.com/w/cpp/language/decltype) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/00-auto-and-decltype.md) | [视频解读](https://www.bilibili.com/video/BV1xkdYYUEyH) | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-0.cpp) |  |


**为什么引入?**

- 解决类型声明过于复杂的问题
- 模板应用中, 获取对象或表达式类型的需求
- 为lambda表达式的定义做支撑

**auto和decltype有什么区别?**

- auto常常用于变量定义, 推导的类型可能丢失const或引用(可显示指定进行保留auto &)
- decltype获取表达式的**精确类型**
- auto通常无法作为模板类型参数使用

## 一、基础用法和场景

### 声明定义

> 充当类型站位符, 辅助变量的定义或声明。使用auto时变量必须要初始化, decltype可以不用初始化

```cpp
int b = 2;
auto b1 = b;
decltype(b) b2 = b;
decltype(b) b3; // 可以不用初始化
```

### 表达式类型推导

> 常常用于复杂表达式的类型推导, 确保计算精度

```c++
int a = 1;

auto b1 = a + 2;
decltype(a + 2 + 1.1) b2 = a + 2 + 1.1;

auto c1 = a + '0';
decltype(2 + 'a') c2 = 2 + 'a';
```

### 复杂类型推导

**迭代器类型推导**

```c++
std::vector<int> v = {1, 2, 3};

auto it = v.begin(); // 自动推导it类型
// decltype(v.begin()) it = v.begin();
for (; it != v.end(); ++it) {
    std::cout << *it << " ";
}
```

**函数类型推导**

> 对于函数或lambda表达式这种复杂类型, 常常使用auto和decltype. 一般, lambda定义用auto,  模板类型参数用decltype

```c++
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

## 二、注意事项 - 括号带来的影响

### decltype(obj) 和 decltype( (obj) )的区别

- 一般`decltype(obj)`获取的时其声明类型
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

## 三、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)