#include <TacticsCrafter/Core/Script.h>

Script::Script(lua_State* lua, const QString& path)
: _lua{lua}
, _name{"Unknown"}
, _version{"0.0.0"}
, _author{"Unknown"}
, _description{"N/A"}
{
    /* Create the script function */
    luaL_loadfile(_lua, path.toStdString().c_str());
    _func = luaL_ref(_lua, LUA_REGISTRYINDEX);

    /* Create the script env */
    lua_newtable(_lua);

    lua_pushstring(_lua, "name");
    lua_pushstring(_lua, _name.toStdString().c_str());
    lua_settable(_lua, -3);

    lua_pushstring(_lua, "version");
    lua_pushstring(_lua, _version.toStdString().c_str());
    lua_settable(_lua, -3);

    lua_pushstring(_lua, "author");
    lua_pushstring(_lua, _author.toStdString().c_str());
    lua_settable(_lua, -3);

    lua_pushstring(_lua, "description");
    lua_pushstring(_lua, _description.toStdString().c_str());
    lua_settable(_lua, -3);

    _env = luaL_ref(_lua, LUA_REGISTRYINDEX);
}

Script::~Script()
{
    luaL_unref(_lua, LUA_REGISTRYINDEX, _func);
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
    _name = luaL_checkstring(_lua, -1);
    lua_pop(_lua, 1);

    lua_pushstring(_lua, "version");
    lua_gettable(_lua, -2);
    _version = luaL_checkstring(_lua, -1);
    lua_pop(_lua, 1);

    lua_pushstring(_lua, "author");
    lua_gettable(_lua, -2);
    _author = luaL_checkstring(_lua, -1);
    lua_pop(_lua, 1);

    lua_pushstring(_lua, "description");
    lua_gettable(_lua, -2);
    _description = luaL_checkstring(_lua, -1);
    lua_pop(_lua, 1);

    lua_pop(_lua, 1);
}
