if is_host("windows") then
    set_encodings("source:utf-8", "target:utf-8")
    set_toolchains("gcc")
    add_ldflags("-static")
end

local lang = "en"
if d2x and d2x.private and d2x.private.mcpp then
    lang = d2x.private.mcpp.lang or "en"
end

if lang == "zh" then

    target("00-0-hello-mcpp")
        set_languages("cxx11")
        add_files("hello-mcpp.cpp")

    includes("cpp11")
else
    includes("en")
end
