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

void Script::setName(const QString& name)
{
    _name = name;
}

void Script::exec()
{
    lua_rawgeti(_lua, LUA_REGISTRYINDEX, _func);
    lua_pcall(_lua, 0, 0, 0);
}
