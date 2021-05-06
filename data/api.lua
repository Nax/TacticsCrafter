Script = {}

Script.properties = function (data)
  for _, k in ipairs({'name', 'description', 'author', 'version'}) do
    local v = data[k]
    if v then
      _script[k] = v
    end
  end
end

Patch = {}

Patch.write8 = function(addr, value)
end

Patch.write16 = function(addr, value)
end

Patch.write32 = function(addr, value)
end
