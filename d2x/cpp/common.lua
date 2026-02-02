function get_local_lang()
--local config = platform.get_config_info()
    local local_lang = "en" -- Default language
    -- TODO: config language by xlings.json
    if is_host("linux") then
        local tmp_local_lang = os.getenv("LANG") or "en"
        if tmp_local_lang:find("zh") then
            local_lang = "zh"
        end
    elseif is_host("windows") then
        local tmp_local_lang = nil
        if find_tool("wmic") then -- Windows 10 and earlier
            -- wmic is deprecated in Windows 11, but still available
            tmp_local_lang = os.iorun([[wmic os get locale]])
        else -- win11+
            tmp_local_lang = os.iorun([[powershell -NoProfile -ExecutionPolicy Bypass -Command "'{0:X4}' -f (Get-Culture).LCID"]])
        end
        if tmp_local_lang and tmp_local_lang:find("0804") then
            local_lang = "zh"
        end
    elseif is_host("macosx") then
        local tmp_local_lang = os.iorun([[defaults read -g AppleLocale]])
        if tmp_local_lang and tmp_local_lang:find("zh") then
            local_lang = "zh"
        end
    end

    return local_lang
end
