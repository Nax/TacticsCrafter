#include <cstdint>
#include <TacticsCrafter/API/API.h>

namespace
{

int api_patch_write8(lua_State* L)
{
    auto state = (State*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = (std::uint32_t)luaL_checkinteger(L, 1);
    std::uint8_t value = (std::uint8_t)luaL_checkinteger(L, 2);

    state->changeset->write8(addr, value);

    return 0;
}

int api_patch_write16(lua_State* L)
{
    auto state = (State*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = (std::uint32_t)luaL_checkinteger(L, 1);
    std::uint16_t value = (std::uint16_t)luaL_checkinteger(L, 2);

    state->changeset->write16(addr, value);

    return 0;
}

int api_patch_write32(lua_State* L)
{
    auto state = (State*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = (std::uint32_t)luaL_checkinteger(L, 1);
    std::uint32_t value = (std::uint32_t)luaL_checkinteger(L, 2);

    state->changeset->write32(addr, value);

    return 0;
}

}

void API::initPatch(lua_State* L, State* state)
{
    static struct luaL_Reg funcs[] = {
        { "write8",  &api_patch_write8  },
        { "write16", &api_patch_write16 },
        { "write32", &api_patch_write32 },
        { nullptr }
    };

    lua_newtable(L);
    lua_pushlightuserdata(L, state);
    luaL_setfuncs(L, funcs, 1);
    lua_setglobal(L, "Patch");
}
