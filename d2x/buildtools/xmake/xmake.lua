task("d2x-buildtools")

    set_category("plugin")

    on_run("main")

    set_menu {
        usage = "xmake d2x-buildtools [options]",
        description = "d2x buildtools plugin",
        options = {
            {nil, "command", "v", nil, "d2x-buildtools's command"},
            {nil, "target", "v", nil, "the target name"}
        }
    }