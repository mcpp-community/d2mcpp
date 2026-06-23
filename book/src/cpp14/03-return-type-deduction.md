<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./03-return-type-deduction.html
[English]: ../en/cpp14/03-return-type-deduction.html

# 返回类型推导 - return type deduction

C++14 允许函数使用 `auto` 作为返回类型而无需后置 `-> decltype(...)`, 编译器从 return 语句自动推导

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-auto](https://en.cppreference.com/w/cpp/language/auto) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/03-return-type-deduction.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/03-return-type-deduction-0.cpp) |  |


**为什么引入?**

- C++11 的 `auto` 返回类型必须配合 `-> decltype(...)` 后置语法
- 很多简单函数不需要显式声明返回类型, 编译器可以从 return 语句推导出来

## 一、基础用法和场景

### auto 返回 — 普通函数

```cpp
auto add(int a, int b) {    // 返回类型推导为 int
    return a + b;
}
```

### auto 返回 — 模板函数

```cpp
template <typename T1, typename T2>
auto multiply(T1 a, T2 b) {  // C++14 不需要 -> decltype(a * b)
    return a * b;
}
```

## 二、真实案例 - STL 中的 auto 返回推导

> MSVC STL 内部大量使用 `auto` 返回类型推导来简化模板函数签名。下面以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 为例 (源码: [`msvc-stl/stl/inc/xutility`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/xutility#L1129-L1132)), `_NODISCARD` / `constexpr` 是库内部标注, 阅读时可忽略

### _To_unsigned_like — auto 返回简化模板函数

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (有删节)
template <_Integer_like _Ty>
_NODISCARD constexpr auto _To_unsigned_like(const _Ty _Value) noexcept {
    return static_cast<_Make_unsigned_like_t<_Ty>>(_Value);
}
```

返回类型是 `_Make_unsigned_like_t<_Ty>`, 如果不写成 `auto`, 函数签名会变成 `_Make_unsigned_like_t<_Ty> _To_unsigned_like(...)`, 返回类型比函数名还长。`auto` 让签名简洁, 返回类型由 return 语句自然表达

## 三、注意事项

### auto 返回必须统一类型

```cpp
auto bad(int x) {
    if (x > 0) return 1;       // int
    else       return 2.0;     // double → 错误!
}
```

### 递归需要显式返回类型

```cpp
auto factorial(int n) {             // 错误: 递归, 无法推导
    return n <= 1 ? 1 : n * factorial(n - 1);
}
```

### auto 返回会剥离引用

需要保留引用时用 `decltype(auto)` (见下一章)

## 四、练习代码

### 练习代码主题

- 0 - [auto 返回类型推导 — 普通函数和模板函数](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/03-return-type-deduction-0.cpp)

### 练习代码自动检测命令

```
d2x checker return-type-deduction
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
