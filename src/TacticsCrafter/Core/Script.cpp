#include <TacticsCrafter/Core/Script.h>

namespace
{

int api_print(lua_State* L)
{
    auto s = (Script*)lua_touserdata(L, lua_upvalueindex(1));
    int nargs = lua_gettop(L);

    for (int i = 1; i <= nargs; ++i)
        s->print(lua_tostring(L, i));

    return 0;
}

}

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

        /* Redirect print */
        lua_pushlightuserdata(_lua, this);
        lua_pushcclosure(_lua, &api_print, 1);
        lua_setglobal(_lua, "print");

        /* Exec the function */
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, _func);
        if (lua_pcall(_lua, 0, 0, 0))
        {
            _log.push_back(lua_tostring(_lua, -1));
        }
    }
}
