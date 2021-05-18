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

}

void API::initScript(lua_State* L, State* state)
{
    static struct luaL_Reg funcs[] = {
        { "properties", &api_script_properties },
        { nullptr }
    };

    lua_newtable(L);
    lua_pushlightuserdata(L, state);
    luaL_setfuncs(L, funcs, 1);
    lua_setglobal(L, "Script");
}
