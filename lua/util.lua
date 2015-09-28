
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

function read_file(path)
	local fd = io.open(path, "r+")
	local fc = fd:read("*a")
	fd:close()
	return fc
end


function write_file(path, fc)
	local fd = io.open(path, "w+")
	fd:write(fc)
	fd:flush()
	fd:close()
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

function cfg_load(path)
	package.loaded[path] = nil

	local ret, etc = xpcall(
	function() 
		return require(path) 
	end, 
	function() 
		print("===================================") 
		print("[找不到配置文件]"..path)
		print("===================================") 
	end)

	if ret and etc then 
		return etc
	else
		return {}
	end
end




function printt(t)
	print(table.tree(t))
end

table.tree = function(root)
	local cache = {  [root] = "." }
	local function _dump(t,space,name)
		local temp = {}
		local keys = {}
		for k,_ in pairs(t) do
			table.insert(keys, k)
		end
		table.sort(keys)

		for _,k in ipairs(keys) do
			local v = t[k]
			local key = tostring(k)
			if cache[v] then
				table.insert(temp,"┣" .. key .. " {" .. cache[v].."}")
			elseif type(v) == "table" then
				local new_key = name .. "." .. key
				cache[v] = new_key
				table.insert(temp,"┣" .. key .. _dump(v,space .. (next(t,k) and "┃" or " " ).. string.rep(" ",#key),new_key))
			else
				table.insert(temp,"┣" .. key .. " [" .. tostring(v).."]")
			end
		end
		return table.concat(temp,"\n"..space)
	end
	return _dump(root, "","")
end


string.split = function(s, p)
	p = p or " "
	local start = 1
	local index = 1
	local array = {}
	while true do
		local i = string.find(s, p, start)
		if i then
			array[index] = string.sub(s, start, i-1)
			start = i + string.len(p)
			index = index + 1
		else
			array[index] = string.sub(s, start, string.len(s))
			break
		end
	end
	return array
end

string.split2 = function(s, p)
    local rt= {}
	string.gsub(s, '[^'..p..']+', function(w) table.insert(rt, w) end)
	return rt
end

string.filename = function(s, keepext)
	if keepext then
		return string.match(s, "*?([^/]*%.%w+)$") -- *nix system
	else
		return string.match(s, "*?([^/]*)%.%w+$") -- *nix system
	end  
end

string.filelast = function(s)
	return string.match(s, "*?([^/]*%w+)$") -- *nix system
end


string.tabstr = function(content, n)
	local str = ""
	local sep = false
	for _,line in ipairs(string.split(content, "\n")) do
		if sep then
			str = str .. "\n" .. string.rep(" ", n) .. line
		else
			str = str .. string.rep(" ", n) .. line
			sep = true
		end
	end

	return str
end



