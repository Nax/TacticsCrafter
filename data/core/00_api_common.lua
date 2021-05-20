Script.properties({
  name = 'CORE: API - Common',
  description = 'TacticsCrafter\'s Common APIs, used by everything else',
  author = 'Nax',
  version = '1.0.0'
})

Util = {}

Util.clone = function (t)
  if type(t) == "table" then
    local t2 = {}
    for k, v in pairs(t) do
      t2[k] = v
    end
    return t2
  else
    return t
  end
end

Util.deep_clone = function (t)
  if type(t) == "table" then
    local t2 = {}
    for k, v in pairs(t) do
      t2[k] = Util.deep_clone(v)
    end
    return t2
  else
    return t
  end
end

-- Load symbols
for k, v in pairs(require('symbols/symbols')) do
  Symbols[k] = v
end
