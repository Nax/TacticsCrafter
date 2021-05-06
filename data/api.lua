Script = {}

Script.properties = function (data)
  for _, k in ipairs({'name', 'description', 'author', 'version'}) do
    local v = data[k]
    if v then
      _script[k] = v
    end
  end
end

function patch_bin8(addr, value)
end

function patch_bin16(addr, value)
end

function patch_bin32(addr, value)
end
