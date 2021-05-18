#include <TacticsCrafter/Core/Script.h>

Script::Script(lua_State* lua, const QString& path, bool core)
: _lua{lua}
, _core{core}
{
    /* Create the script function */
    if (luaL_loadfile(_lua, path.toStdString().c_str()))
    {
        _func = LUA_NOREF;
        _log.push_back(lua_tostring(_lua, -1));
    }
    else
    {
        _func = luaL_ref(_lua, LUA_REGISTRYINDEX);
    }
}

Script::~Script()
{
    if (_func != LUA_NOREF)
        luaL_unref(_lua, LUA_REGISTRYINDEX, _func);
}

void Script::exec()
{
    if (_func != LUA_NOREF)
    {
        /* Clear the log */
        _log.clear();

        /* Exec the function */
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, _func);
        if (lua_pcall(_lua, 0, 0, 0))
        {
            _log.push_back(lua_tostring(_lua, -1));
        }
    }
}
