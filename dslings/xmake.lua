if is_host("windows") then
    set_encodings("source:utf-8", "target:utf-8")
    set_toolchains("gcc")
    add_ldflags("-static")
end

option("lang")
    set_default("en")
    set_description("Language: en or zh")

local lang = get_config("lang")

if lang == "zh" then

    target("00-0-hello-mcpp")
        set_languages("cxx11")
        add_files("hello-mcpp.cpp")

    includes("cpp11")
else
    includes("en")
end
