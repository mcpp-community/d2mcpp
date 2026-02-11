if is_host("windows") then
    set_languages("cxx14")
else
    set_languages("cxx11")
end

-- target: cpp11-00-auto-and-decltype

target("cpp11-00-auto-and-decltype")
    set_kind("binary")
    add_files("00-auto-and-decltype-0.cpp")
target("cpp11-00-auto-and-decltype-1")
    set_kind("binary")
    add_files("00-auto-and-decltype-1.cpp")
target("cpp11-00-auto-and-decltype-2")
    set_kind("binary")
    add_files("00-auto-and-decltype-2.cpp")
target("cpp11-00-auto-and-decltype-3")
    set_kind("binary")
    add_files("00-auto-and-decltype-3.cpp")
target("cpp11-00-auto-and-decltype-4")
    set_kind("binary")
    add_files("00-auto-and-decltype-4.cpp")

-- target: cpp11-01-default-and-delete

target("cpp11-01-default-and-delete-0")
    set_kind("binary")
    add_files("01-default-and-delete-0.cpp")

target("cpp11-01-default-and-delete-1")
    set_kind("binary")
    add_files("01-default-and-delete-1.cpp")

target("cpp11-01-default-and-delete-2")
    set_kind("binary")
    add_files("01-default-and-delete-2.cpp")

-- target: cpp11-02-final-and-override

target("cpp11-02-final-and-override-0")
    add_files("02-final-and-override-0.cpp")

target("cpp11-02-final-and-override-1")
    add_files("02-final-and-override-1.cpp")

target("cpp11-02-final-and-override-2")
    add_files("02-final-and-override-2.cpp")

-- target: cpp11-03-trailing-return-type

target("cpp11-03-trailing-return-type")
    add_files("03-trailing-return-type.cpp")

-- target: cpp11-04-rvalue-references

target("cpp11-04-rvalue-references")
    set_optimize("none")
    add_cxxflags("-fno-elide-constructors")
    add_files("04-rvalue-references.cpp")

-- target: cpp11-05-move-semantics

target("cpp11-05-move-semantics-0")
    add_files("05-move-semantics-0.cpp")

target("cpp11-05-move-semantics-1")
    add_files("05-move-semantics-1.cpp")

target("cpp11-05-move-semantics-2")
    add_files("05-move-semantics-2.cpp")

-- target: cpp11-06-scoped-enums

target("cpp11-06-scoped-enums-0")
    add_files("06-scoped-enums-0.cpp")

target("cpp11-06-scoped-enums-1")
    add_files("06-scoped-enums-1.cpp")

-- target: cpp11-07-constexpr

target("cpp11-07-constexpr-0")
    add_cxxflags("-Wpedantic -Werror")
    add_files("07-constexpr-0.cpp")

target("cpp11-07-constexpr-1")
    add_files("07-constexpr-1.cpp")

-- target: cpp11-08-literal-type

target("cpp11-08-literal-type-0")
    set_languages("c++17") -- TODO: optimize it
    add_files("08-literal-type-0.cpp")

target("cpp11-08-literal-type-1")
    add_files("08-literal-type-1.cpp")

-- target: cpp11-09-list-initialization

target("cpp11-09-list-initialization-0")
    add_files("09-list-initialization-0.cpp")

target("cpp11-09-list-initialization-1")
    add_files("09-list-initialization-1.cpp")

target("cpp11-09-list-initialization-2")
    add_files("09-list-initialization-2.cpp")

target("cpp11-09-list-initialization-3")
    add_files("09-list-initialization-3.cpp")

-- target: cpp11-10-delegating-constructors

target("cpp11-10-delegating-constructors-0")
    add_files("10-delegating-constructors-0.cpp")

target("cpp11-10-delegating-constructors-1")
    add_files("10-delegating-constructors-1.cpp")

-- target: cpp11-11-inherited-constructors

target("cpp11-11-inherited-constructors-0")
    add_files("11-inherited-constructors-0.cpp")

target("cpp11-11-inherited-constructors-1")
    add_files("11-inherited-constructors-1.cpp")

target("cpp11-11-inherited-constructors-2")
    add_files("11-inherited-constructors-2.cpp")

-- target: cpp11-12-nullptr

target("cpp11-12-nullptr-0")
    add_files("12-nullptr-0.cpp")

target("cpp11-12-nullptr-1")
    add_files("12-nullptr-1.cpp")

target("cpp11-12-nullptr-2")
    add_files("12-nullptr-2.cpp")

-- target: cpp11-13-long-long
target("cpp11-13-long-long-0")
    add_files("13-long-long-0.cpp")

target("cpp11-13-long-long-1")
    add_files("13-long-long-1.cpp")

-- target: cpp11-14-type-alias

target("cpp11-14-type-alias-0")
    add_files("14-type-alias-0.cpp")

target("cpp11-14-type-alias-1")
    add_files("14-type-alias-1.cpp")

target("cpp11-14-type-alias-2")
    add_files("14-type-alias-2.cpp")

target("cpp11-14-type-alias-3")
    add_files("14-type-alias-3.cpp")

-- target: cpp11-15-variadic-templates

target("cpp11-15-variadic-templates-0")
    add_files("15-variadic-templates-0.cpp")

target("cpp11-15-variadic-templates-1")
    add_files("15-variadic-templates-1.cpp")

-- target: cpp11-17-pod-type

target("cpp11-17-pod-type-0")
    add_files("17-pod-type-0.cpp")

target("cpp11-17-pod-type-1")
    add_files("17-pod-type-1.cpp")

target("cpp11-17-pod-type-2")
    add_files("17-pod-type-2.cpp")
