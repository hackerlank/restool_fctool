require("util")


function getset(msh, dsc)
    if not dsc then
        return {msh = msh}
    end


    local fd = io.open("../out/zhujue/girl/"..string.lower(dsc), "r")
    if not fd then
        fd = io.open("../out/zhujue/girl/"..dsc, "r")
    end
    if not fd then
        return {msh = msh}
    end

    msh = "out/zhujue/girl/"..string.lower(msh)
    msh = string.gsub(msh, ".cmf", ".skin")

    local txt = fd:read("*a")
    fd:close()

    local tex = string.match(txt, "%(\"(.+[,)]\"%)")
    if tex then
        tex = string.lower(tex) .. ".png"
    else
        tex = string.filename(msh) .. ".png"
    end

    local tt = string.filename(msh, true)
    tt = string.gsub(tt, "-", "%%-")
    tex = string.gsub(msh, tt, tex)

    return {msh = msh, tex = tex}
end


local name = {}
local part = {}
local suit = {}
local xml = require("xmlSimple").newParser():loadFile("girl.xml")

for i,v in ipairs(xml.root.components:children()) do
    print(v:name(), v:numChildren())
    table.insert(name, v:name())
    local pcfg = {}
    part[v:name()] = pcfg
    for i,c in ipairs(v:children()) do
        local id = tonumber(c["@id"])
        local comp = suit[id] or {}
        suit[id] = comp

        local set = getset(c["@mesh"], c["@skin"])
        comp[v:name()] = set
        pcfg[id] = set
    end
end

local ids = {}
for i,v in pairs(suit) do
    print("["..i.."]-----------------------")
    table.insert(ids, i)
    for k,c in pairs(v) do
        print(k, c.msh, c.tex)
    end
end
table.sort(ids)


local cfg = {}
cfg.ids = ids
cfg.name = name
cfg.suit = suit
cfg.part = part
cfg_save("xml.lua", cfg)








