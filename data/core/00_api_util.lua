Script.properties({
  name = 'CORE: API - Util',
  description = 'TacticsCrafter\'s Utilities API',
  author = 'Nax',
  version = '1.0.0'
})

Util = {}

Util.clone = function (t)
  local t2 = {}
  for k, v in pairs(t) do
    t2[k] = v
  end
  return t2
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
