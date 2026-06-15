set_languages("cxx14")

-- target: cpp14-00-generic-lambdas

target("cpp14-00-generic-lambdas-0-ref")
    set_kind("binary")
    add_files("00-generic-lambdas-0.cpp")

target("cpp14-00-generic-lambdas-1-ref")
    set_kind("binary")
    add_files("00-generic-lambdas-1.cpp")
