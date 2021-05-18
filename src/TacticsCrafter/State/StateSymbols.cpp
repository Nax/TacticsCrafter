#include <TacticsCrafter/State/StateSymbols.h>

static const char* kRegistryName = "TC/Symbol";

static int api_symbol_meta_index(lua_State* L)
{
    auto symbols = *((StateSymbols**)luaL_checkudata(L, 1, kRegistryName));
    const char* k = luaL_checkstring(L, 2);

    auto it = symbols->syms.find(k);
    if (it == symbols->syms.end())
        lua_pushnil(L);
    else
        lua_pushnumber(L, it->second);
    return 1;
}

static int api_symbol_meta_newindex(lua_State* L)
{
    auto symbols = *((StateSymbols**)luaL_checkudata(L, 1, kRegistryName));
    const char* k = luaL_checkstring(L, 2);
    std::uint32_t v = luaL_checknumber(L, 3);

    symbols->syms[k] = v;
    return 0;
}

StateSymbols::StateSymbols()
{
}

void StateSymbols::apply(lua_State* L)
{
    static struct luaL_Reg funcs[] = {
        { "__index", &api_symbol_meta_index },
        { "__newindex", &api_symbol_meta_newindex },
        { nullptr }
    };

    /* Userdata */
    auto t = (StateSymbols**)lua_newuserdata(L, sizeof(this));
    *t = this;

    /* Metatable */
    if (luaL_newmetatable(L, kRegistryName))
        luaL_setfuncs(L, funcs, 0);
    lua_setmetatable(L, -2);

    /* Global */
    lua_setglobal(L, "Symbol");
}
