#include <TacticsCrafter/Core/Script.h>

Script::Script(lua_State* lua, const QString& path)
: _lua{lua}
, _name{"Unknown"}
{
    luaL_loadfile(_lua, path.toStdString().c_str());
    _func = luaL_ref(_lua, LUA_REGISTRYINDEX);
}

Script::~Script()
{
    luaL_unref(_lua, LUA_REGISTRYINDEX, _func);
}
