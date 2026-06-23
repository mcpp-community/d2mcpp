<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./07-deprecated-attribute.html
[English]: ../en/cpp14/07-deprecated-attribute.html

# deprecated 属性

C++14 引入 `[[deprecated]]` 属性, 允许标记废弃的函数、类或变量, 编译时产生警告

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-attribute](https://en.cppreference.com/w/cpp/language/attributes) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/07-deprecated-attribute.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/07-deprecated-attribute-0.cpp) |  |


**为什么引入?**

- C++11 之前没有标准方式标记废弃 API, 只能靠文档或 `#warning` 等非标准手段
- `[[deprecated]]` 在编译期产生警告, 调用方无法忽略, 比注释或文档更有效

## 一、基础用法和场景

```cpp
[[deprecated("Use new_api() instead")]]
void old_api() { }

[[deprecated]]
int legacy_value = 42;

void modern_code() {
    old_api();       // 编译器警告: old_api is deprecated
    int x = legacy_value;  // 编译器警告: legacy_value is deprecated
}
```

## 二、真实案例 - STL 中的 [[deprecated]]

> MSVC STL 使用 `[[deprecated]]` 包装废弃头文件和 API 的警告宏。下面以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 为例 (源码: [`msvc-stl/stl/inc/yvals_core.h`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/yvals_core.h#L1057-L1061))

```cpp
// MSVC STL · msvc-stl/stl/inc/yvals_core.h (有删节)
#define _CXX17_DEPRECATE_C_HEADER \
    [[deprecated("warning STL4004: " \
                 "<ccomplex>, <cstdalign>, <cstdbool>, and <ctgmath> " \
                 "are deprecated in C++17.")]]
```

STL 将 `[[deprecated]]` 封装成宏, 在编译期对使用废弃 C 头文件的代码产生标准警告, 用户可借此提前迁移

## 三、注意事项

- 可标记: 函数、类、变量、枚举、using 别名
- 消息字符串可选但推荐
- 废弃不等于删除 — 编译器仍正常生成代码

## 四、练习代码

### 练习代码主题

- 0 - [[[deprecated]] 属性 — 标记废弃函数](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/07-deprecated-attribute-0.cpp)

### 练习代码自动检测命令

```
d2x checker deprecated-attribute
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
