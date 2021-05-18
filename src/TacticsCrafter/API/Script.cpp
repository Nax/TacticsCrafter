#include <TacticsCrafter/API/API.h>

namespace
{

int api_script_properties(lua_State* L)
{
    const char* tmp;

    auto state = (State*)lua_touserdata(L, lua_upvalueindex(1));
    lua_getfield(L, 1, "name");
    if (tmp = luaL_checkstring(L, -1))
        state->scriptMeta.name = tmp;
    lua_pop(L, 1);
    lua_getfield(L, 1, "version");
    if (tmp = luaL_checkstring(L, -1))
        state->scriptMeta.version = tmp;
    lua_pop(L, 1);
    lua_getfield(L, 1, "author");
    if (tmp = luaL_checkstring(L, -1))
        state->scriptMeta.author = tmp;
    lua_pop(L, 1);
    lua_getfield(L, 1, "description");
    if (tmp = luaL_checkstring(L, -1))
        state->scriptMeta.description = tmp;
    lua_pop(L, 1);

    return 0;
}

int api_script_opt_bool(lua_State* L)
{
    auto state = (State*)lua_touserdata(L, lua_upvalueindex(1));
    const char* key = luaL_checkstring(L, 1);
    const char* text = luaL_checkstring(L, 2);
    bool value = lua_isnone(L, 3) ? false : lua_toboolean(L, 3);

    lua_pushboolean(L, state->script->optBool(key, text, value));

    return 1;
}

}

void API::initScript(lua_State* L, State* state)
{
    static struct luaL_Reg funcs[] = {
        { "properties", &api_script_properties },
        { "opt_bool", &api_script_opt_bool },
        { nullptr }
    };

    lua_newtable(L);
    lua_pushlightuserdata(L, state);
    luaL_setfuncs(L, funcs, 1);
    lua_setglobal(L, "Script");
}
