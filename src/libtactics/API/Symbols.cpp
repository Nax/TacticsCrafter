#include <libtactics/API.h>

namespace
{

int api_symbol_meta_index(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));

    const char* k = luaL_checkstring(L, 2);

    auto it = ctx->symbols.find(k);
    if (it == ctx->symbols.end())
        lua_pushnil(L);
    else
        lua_pushnumber(L, it->second);
    return 1;
}

int api_symbol_meta_newindex(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));

    const char* k = luaL_checkstring(L, 2);
    std::uint32_t v = (std::uint32_t)luaL_checknumber(L, 3);

    ctx->symbols[k] = v;
    return 0;
}

}

void API::initSymbols(LTC_Context* ctx)
{
    static struct luaL_Reg funcs[] = {
        { "__index", &api_symbol_meta_index },
        { "__newindex", &api_symbol_meta_newindex },
        { nullptr }
    };

    lua_State* L = ctx->L;

    lua_newtable(L);
    lua_newtable(L);
    lua_pushlightuserdata(L, ctx);
    luaL_setfuncs(L, funcs, 1);
    lua_setmetatable(L, -2);
    lua_setglobal(L, "Symbols");
}
