#include <cstdint>
#include <libtactics/API.h>

namespace
{

int api_memory_alloc(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = 0x08804000 + 0xfe5200 + ctx->extraMemory;
    std::uint32_t len = (std::uint32_t)luaL_checkinteger(L, 1);

    if (len > 0x100000)
    {
        lua_pushnil(L);
    }
    else
    {
        len = (len + 3) & 0xfffffffc;
        lua_pushinteger(L, addr);
        ctx->extraMemory += len;
    }
    return 1;
}

}

void API::initMemory(LTC_Context* ctx)
{
    static struct luaL_Reg funcs[] = {
        { "alloc",  &api_memory_alloc },
        { nullptr }
    };

    lua_State* L = ctx->L;

    lua_newtable(L);
    lua_pushlightuserdata(L, ctx);
    luaL_setfuncs(L, funcs, 1);
    lua_setglobal(L, "Memory");
}
