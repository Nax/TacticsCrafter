#include <TacticsCrafter/API/API.h>

namespace
{

int api_symbol_meta_index(lua_State* L)
{
    auto state = (State*)lua_touserdata(L, lua_upvalueindex(1));
    const char* k = luaL_checkstring(L, 2);

    auto it = state->symbols.find(k);
    if (it == state->symbols.end())
        lua_pushnil(L);
    else
        lua_pushnumber(L, it->second);
    return 1;
}

int api_symbol_meta_newindex(lua_State* L)
{
    auto state = (State*)lua_touserdata(L, lua_upvalueindex(1));
    const char* k = luaL_checkstring(L, 2);
    std::uint32_t v = luaL_checknumber(L, 3);

    state->symbols[k] = v;
    return 0;
}

}

void API::initSymbols(lua_State* L, State* state)
{
    static struct luaL_Reg funcs[] = {
        { "__index", &api_symbol_meta_index },
        { "__newindex", &api_symbol_meta_newindex },
        { nullptr }
    };

    lua_newtable(L);
    lua_newtable(L);
    lua_pushlightuserdata(L, state);
    luaL_setfuncs(L, funcs, 1);
    lua_setmetatable(L, -2);
    lua_setglobal(L, "Symbols");
}
