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
, _error{}
{
    /* Create the script function */
    if (luaL_loadfile(_lua, path.toStdString().c_str()))
    {
        _func = LUA_NOREF;
        _log.push_back(lua_tostring(_lua, -1));
        _error = true;
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

bool Script::optBool(const QString& key, const QString& text, bool value)
{
    Option* o;

    o = optLookup(key, Option::Type::Bool);
    if (!o)
    {
        _opts.resize(_opts.size() + 1);
        o = &_opts.back();
        o->type = Option::Type::Bool;
        o->key = key;
        o->b = value;
    }
    o->text = text;
    return o->b;
}

void Script::exec()
{
    if (_func != LUA_NOREF)
    {
        /* Clear the log & error flag */
        _log.clear();
        _error = false;

        /* Redirect print */
        lua_pushlightuserdata(_lua, this);
        lua_pushcclosure(_lua, &api_print, 1);
        lua_setglobal(_lua, "print");

        /* Exec the function */
        lua_rawgeti(_lua, LUA_REGISTRYINDEX, _func);
        if (lua_pcall(_lua, 0, 0, 0))
        {
            _log.push_back(lua_tostring(_lua, -1));
            _error = true;
        }
    }
}

Script::Option* Script::optLookup(const QString& key, Option::Type type)
{
    int index = -1;
    for (int i = 0; i < _opts.size(); ++i)
    {
        Option& o = _opts[i];
        if (o.key == key)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        return nullptr;

    if (_opts[index].type != type)
    {
        _opts.erase(_opts.begin() + index);
        return nullptr;
    }

    return _opts.data() + index;
}
