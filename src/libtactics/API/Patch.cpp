#include <cstdint>
#include <libtactics/API.h>

namespace
{

int api_patch_write8(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = (std::uint32_t)luaL_checkinteger(L, 1);
    std::uint8_t value = (std::uint8_t)luaL_checkinteger(L, 2);

    ctx->changeset.write8(addr, value);

    return 0;
}

int api_patch_write16(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = (std::uint32_t)luaL_checkinteger(L, 1);
    std::uint16_t value = (std::uint16_t)luaL_checkinteger(L, 2);

    ctx->changeset.write16(addr, value);

    return 0;
}

int api_patch_write32(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = (std::uint32_t)luaL_checkinteger(L, 1);
    std::uint32_t value = (std::uint32_t)luaL_checkinteger(L, 2);

    ctx->changeset.write32(addr, value);

    return 0;
}

int api_patch_string(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = (std::uint32_t)luaL_checkinteger(L, 1);
    const char* str = luaL_checkstring(L, 2);

    ctx->changeset.blob(addr, str, std::strlen(str));

    return 0;
}

int api_patch_cstring(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = (std::uint32_t)luaL_checkinteger(L, 1);
    const char* str = luaL_checkstring(L, 2);

    ctx->changeset.blob(addr, str, std::strlen(str) + 1);

    return 0;
}

}

void API::initPatch(LTC_Context* ctx)
{
    static struct luaL_Reg funcs[] = {
        { "write8",   &api_patch_write8  },
        { "write16",  &api_patch_write16 },
        { "write32",  &api_patch_write32 },
        { "string",   &api_patch_string  },
        { "cstring",  &api_patch_cstring },
        { nullptr }
    };

    lua_State* L = ctx->L;

    lua_newtable(L);
    lua_pushlightuserdata(L, ctx);
    luaL_setfuncs(L, funcs, 1);
    lua_setglobal(L, "Patch");
}
