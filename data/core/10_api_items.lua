Script.properties({
  name = 'CORE: API - Items',
  description = 'TacticsCrafter\'s Item API',
  author = 'Nax',
  version = '1.0.0'
})

-- Load item data
Item = {}
Item.data = Util.deep_clone(require('items/data'))
Item.count = 0x13c

-- Register item IDs
local item_ids = require('items/id')
for k, v in pairs(item_ids) do
  Registry["item:fft/" .. k] = v
end

local patch_item_propx = function(id, offset, value, setter)
  local base = 0
  if id < 0x100 then
    base = 0x08b29288
  else
    base = 0x08a5adac
    id = id - 0x100
  end
  local addr = base + id * 12 + offset
  setter(addr, value)
end

local patch_item_prop16 = function(id, offset, value)
  patch_item_propx(id, offset, value, Patch.write16)
end

local patch_item_prop8 = function(id, offset, value)
  patch_item_propx(id, offset, value, Patch.write8)
end

local ItemPropertySetters = {
  palette = function (t, v) t._data.palette = v; patch_item_prop8(t.id, 0, v) end,
  sprite = function (t, v) t._data.sprite = v; patch_item_prop8(t.id, 1, v) end,
  level = function (t, v) t._data.level = v; patch_item_prop8(t.id, 2, v) end,
  type = function (t, v) t._data.type = v; patch_item_prop8(t.id, 5, v) end,
  price = function (t, v) t._data.price = v; patch_item_prop16(t.id, 8, v) end,
  shop = function (t, v) t._data.shop = v; patch_item_prop8(t.id, 10, v) end,
}

local function get_item_property(t, k)
  return t._data[k]
end

local set_item_property = function (t, k, v)
  local f = ItemPropertySetters[k]
  if f then
    f(t, v)
  end
end

local ItemRefMetatable = {
  __index = get_item_property,
  __newindex = set_item_property,
}

local get_item_id = function (id)
  if type(id) == "string" then
    local id2 = Registry["item:" .. id]
    if id2 == nil then
      id2 = Registry["item:fft/" .. id]
    end
    if id2 == nil then
      error("Unknown item: " .. id)
    end
    return id2
  else
    return id
  end
end

local get_item = function (_, key)
  local id = get_item_id(key)
  local data = Item.data[id]
  data.id = id
  local item = { _data = data }
  setmetatable(item, ItemRefMetatable)
  return item
end

-- Setup the Items API
Items = {}
setmetatable(Items, {
  __index = get_item
})
