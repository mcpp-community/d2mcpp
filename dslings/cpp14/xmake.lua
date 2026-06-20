set_languages("cxx14")

-- target: cpp14-00-generic-lambdas

target("cpp14-00-generic-lambdas-0")
    set_kind("binary")
    add_files("00-generic-lambdas-0.cpp")

target("cpp14-00-generic-lambdas-1")
    set_kind("binary")
    add_files("00-generic-lambdas-1.cpp")

target("cpp14-01-relaxed-constexpr-0")
    set_kind("binary")
    add_files("01-relaxed-constexpr-0.cpp")

target("cpp14-01-relaxed-constexpr-1")
    set_kind("binary")
    add_files("01-relaxed-constexpr-1.cpp")
