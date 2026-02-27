import("core.base.json")
import("core.base.option")
import("core.project.project")
import("core.project.config")

local function get_d2x_lang()
    local d2x_json_file = path.join(os.projectdir(), ".d2x.json")
    if os.isfile(d2x_json_file) then
        local d2x_config = json.loadfile(d2x_json_file) or {}
        return d2x_config["lang"]
    end
    return nil
end

function list()

    config.load()
    local lang = get_d2x_lang()
    if lang and #lang > 0 then
        config.set("lang", lang, {force = true})
    end

    local targets = project.targets()
    for name, _ in pairs(targets) do
        local files = targets[name]:sourcefiles()
        local flag = true

        -- cprintf / print xxx
        printf(name) -- target name
        for _, file in ipairs(files) do
            file = path.absolute(file)
            if flag then
                --printf(": " .. file) -- avoid C:\ issue on Windows
                printf("@ " .. file)
                flag = false
            else
                printf(", " .. file)
            end
        end
        printf("\n")
    end
end

function main()

    local command = option.get("command")
    local target = option.get("target")

    os.cd(os.projectdir())
    --print("project file: " .. project.rootfile())

    if command == "init" then
        local lang = get_d2x_lang()
        if lang and #lang > 0 then
            os.exec("xmake f --lang=" .. lang)
        end
    elseif command == "list" then
        list()
    elseif command == "build" then
        os.exec("xmake build " .. (target or ""))
    elseif command == "run" then
        os.exec("xmake run " .. (target or ""))
    else
        print("Unknown command: " .. tostring(command))
    end

end
