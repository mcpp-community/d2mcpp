<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/10-delegating-constructors.html
[English]: ./10-delegating-constructors.html

# Delegating Constructors

Delegating constructors are syntactic sugar introduced in C++11. Through simple syntax, they can avoid writing excessive repetitive code and achieve constructor logic reuse without affecting performance.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/initializer_list.html#Delegating_constructor) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/10-delegating-constructors.md) | [Video Explanation](https://www.bilibili.com/video/BV1zft3zSEER) | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/10-delegating-constructors-0.cpp)  |  |

**Why was it introduced?**

- Avoid writing repetitive code in constructor overloading
- Facilitate code maintenance

## I. Basic Usage and Scenarios

### Reusing Constructor Logic

When a class needs to write overloaded constructors, it's easy to end up with a lot of repetitive code, for example:

```cpp
class Account {
    string id;
    string name;
    string coin;
public:

    Account(string id_) {
        id = id_;
        name = "momo";
        coin = "0元";
    }

    Account(string id_, string name_) {
        id = id_;
        name = name_;
        coin = "0元";
    }

    Account(string id_, string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";
    }
};
```

The initialization code in these 3 constructors is clearly repetitive (actual initialization might be more complex). With **delegating constructor** support, by using ` : Account(xxx) ` in the **constructor member initialization list** to delegate to other more complete constructors, we can keep only one copy of the code.

```cpp
class Account {
    string id;
    string name;
    string coin;
public:

    Account(string id_) : Account(id_, "momo") { }

    Account(string id_, string name_) : Account(id_, name_, 0) { }

    Account(string id_, string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";
    }
};
```

The above two constructors, through delegation, will ultimately forward to `Account(string id_, string name_, int coin_)`.

### Why is it easier to maintain?

Suppose if the currency unit or name needs to be modified, the repetitive code implementation not only violates the reuse principle but also requires multiple modifications when changing constructor logic, increasing maintenance costs.

With delegating constructors, the constructor logic is placed in one location, making modifications and maintenance more convenient.

For example, if we need to change `元` to `原石`, we only need to modify it once:

```cpp
class Account {
    // ...
    Account(string id_, string name_, int coin_) {
        //...
        //coin = std::to_string(coin_) + "元";
        coin = std::to_string(coin_) + "原石";
    }
};
```

### Difference from encapsulating in an init function

Some might think: if we write the constructor logic as an `init` function, wouldn't that also achieve code reuse? Why add a new syntax as a feature to the standard? Isn't it redundant and making C++ more complex?

```cpp
class Account {
    // ...

    init(string id_, string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";
    }

public:

    Account(string id_) { init(id_, "momo", 0); }

    Account(string id_, string name_) { init(id_, name_, 0); }

    Account(string id_, string name_, int coin_) {
        init(id_, name_, coin_);
    }
};
```

Actually, from a performance perspective, in most cases, separately encapsulating an `init` function has lower performance than **delegating constructors**. Because member construction generally goes through two stages:

- Step 1: Execute default initialization or member initialization list
- Step 2: Run constructor logic in the constructor body

```cpp
class Account {
    // ...
public:

    Account(string id_, string name_, int coin_)
        /* : 1 - member initialization list */
    {
        // 2 - execute constructor function body
        init(id_, name_, coin_);
    }
};
```

This causes members to be "initialized" twice when using an init function, while **delegating constructors** can avoid this problem through the **member initialization list**:

```cpp
class Account {
    // ...
public:

    Account(string id_, string name_, int coin_)
        : id { id_ }, name { name_ }, coin { std::to_string(coin_) + "元" }
    {
        // ...
    }
};
```

## II. Important Notes

### Temporary Object Misunderstanding

In scenarios not using delegating constructors, calling another constructor within a constructor body actually just creates a temporary object:

- Calling a normal function `init`: initializes this object's members
- Calling another constructor: creates a new temporary object outside this object

```cpp
class Account {
    // ...
public:

    Account(string id_, string name_) {
        Account(id_, name_, 0); // creates a temporary object
        // init(id_, name_, 0);
        // this->Account(id_, name_, 0); // error
    }

    Account(string id_, string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";
    }
};

```

### Cannot Reinitialize

When using **delegating constructors**, you cannot use the initialization list to initialize other members. This restriction avoids repeated initialization and ensures data members are only initialized once.

For example, if the following syntax were allowed, `coin` would be initialized multiple times and could cause ambiguity:

```cpp
class Account {
    // ...
public:

    Account(string id_)
        : Account(id_, "momo"), coin { "0元" } // error
    {

    }

};
```

## III. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
