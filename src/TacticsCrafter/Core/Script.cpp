#include <TacticsCrafter/Core/Script.h>

Script::Script(lua_State* lua, const QString& path, bool core)
: _lua{lua}
, _core{core}
{
    /* Create the script function */
    luaL_loadfile(_lua, path.toStdString().c_str());
    _func = luaL_ref(_lua, LUA_REGISTRYINDEX);
}

Script::~Script()
{
    luaL_unref(_lua, LUA_REGISTRYINDEX, _func);
}

void Script::exec()
{
    /* Exec the function */
    lua_rawgeti(_lua, LUA_REGISTRYINDEX, _func);
    lua_pcall(_lua, 0, 0, 0);
}
