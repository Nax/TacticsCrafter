#include <TacticsCrafter/Core/Script.h>

Script::Script(lua_State* lua, const QString& path)
: _lua{lua}
, _name{"Unknown"}
{
    /* Create the script function */
    luaL_loadfile(_lua, path.toStdString().c_str());
    _func = luaL_ref(_lua, LUA_REGISTRYINDEX);

    /* Create the script env */
    lua_newtable(_lua);
    lua_pushstring(_lua, "name");
    lua_pushstring(_lua, "Unknown");
    lua_settable(_lua, -3);
    lua_pushstring(_lua, "description");
    lua_pushstring(_lua, "Unknown");
    lua_settable(_lua, -3);
    _env = luaL_ref(_lua, LUA_REGISTRYINDEX);
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
    /* Load the env */
    lua_rawgeti(_lua, LUA_REGISTRYINDEX, _env);
    lua_setglobal(_lua, "_script");

    /* Exec the function */
    lua_rawgeti(_lua, LUA_REGISTRYINDEX, _func);
    lua_pcall(_lua, 0, 0, 0);

    /* Process the mutated env */
    lua_rawgeti(_lua, LUA_REGISTRYINDEX, _env);
    lua_pushstring(_lua, "name");
    lua_gettable(_lua, -2);
    auto* name = luaL_checkstring(_lua, -1);
    _name = name;
    lua_pop(_lua, 2);
}
