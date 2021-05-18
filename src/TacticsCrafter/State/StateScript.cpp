#include <TacticsCrafter/State/StateScript.h>

static int api_script_properties(lua_State* L)
{
    const char* tmp;

    auto script = (StateScript*)lua_touserdata(L, lua_upvalueindex(1));
    lua_getfield(L, 1, "name");
    if (tmp = luaL_checkstring(L, -1))
        script->meta.name = tmp;
    lua_pop(L, 1);
    lua_getfield(L, 1, "version");
    if (tmp = luaL_checkstring(L, -1))
        script->meta.version = tmp;
    lua_pop(L, 1);
    lua_getfield(L, 1, "author");
    if (tmp = luaL_checkstring(L, -1))
        script->meta.author = tmp;
    lua_pop(L, 1);
    lua_getfield(L, 1, "description");
    if (tmp = luaL_checkstring(L, -1))
        script->meta.description = tmp;
    lua_pop(L, 1);

    return 0;
}

StateScript::StateScript()
{
    reset();
}

void StateScript::reset()
{
    meta.name = "Unknown";
    meta.version = "0.0.0";
    meta.author = "Unknown";
    meta.description = "N/A";
}

void StateScript::apply(lua_State* L)
{
    static struct luaL_Reg funcs[] = {
        { "properties", &api_script_properties },
        { nullptr }
    };

    lua_newtable(L);
    lua_pushlightuserdata(L, this);
    luaL_setfuncs(L, funcs, 1);
    lua_setglobal(L, "Script");
}
