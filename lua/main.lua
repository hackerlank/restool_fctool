

function exec(cmd, flag)
    if flag then
        return os.execute(cmd)
    end

	local ret = {}
	for v in io.popen(cmd):lines() do  --Linux 
		table.insert(ret, v)
	end
	return ret 
end


function check_file(path)
	local fd = io.open(path, "r")
    if fd then
        fd:close()
        return true
    else
        return false
    end
end

function cfg_save(path, data, text)
	function _key(k)
		if type(k) == "number" then
			return "[" .. k .. "]"
		else
			return "[\"" .. k .. "\"]"
	    end
	end
	function _val(v, dep)
		if type(v) == "table" then
			return _tab(v, dep + 1)
		elseif type(v) == "string" then
			return "\"" .. v .. "\""
		else
			return tostring(v)
		end
	end
	function _tab(tab, dep)
		dep = dep or 0
		local pre = ""
		for i = 1, dep do
			pre = pre .. "    "
		end

		local str = "{\n"

		local cnt = 0
		for k,v in pairs(tab) do
			cnt = cnt + 1
		end

		if cnt == #tab then
			for i,v in ipairs(tab) do
				str = str .. pre .. "    " .. _val(v, dep)  .. ",\n"
			end
		else
			for k,v in pairs(tab) do
				str = str .. pre .. "    " .. _key(k) .. " = " .. _val(v, dep)  .. ",\n"
			end
		end
		str = str .. pre .. "}"
		return str
	end
	
	local fd = io.open(path, "w+")
	fd:write((text or "return ") .. _tab(data) .. "\n")
	fd:flush()
	fd:close()
end

function scan(dir)
    local bone = exec("find "..dir.." -name *.bone")
    if #bone ~= 1 then
        return nil
    end
    local cfg = {}
    cfg.bone = bone[1]
    cfg.skel = exec("find "..dir.." -name *.skel")
    cfg.part = {}

    local sub = exec("find "..dir.." -type d")
    for t,d in ipairs(sub) do
        local p = {}
        cfg.part[t - 1] = p
        
        local skin = exec("find "..d.." -name *.skin")

        for i,v in ipairs(skin) do
            local tex = string.gsub(v, ".skin", ".png")
            if check_file(tex) then
                p[i] = {v, tex}
            else 
                tex = string.gsub(tex, "_", "-")
                if check_file(tex) then
                    p[i] = {v, tex}
                else
                    p[i] = {v, nil}
                end
            end
            --print(p[i][1], p[i][2])
        end
        print(t, #p)
    end
    return cfg
end



local dir = ... or "./"
local cfg = scan(dir)
cfg_save("cfg.lua", cfg)







