#include <cstdint>
#include <TacticsCrafter/API/API.h>
#include <TacticsCrafter/Core/Assembler.h>

namespace
{

int api_asm_patch(lua_State* L)
{
    auto state = (State*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = (std::uint32_t)luaL_checkinteger(L, 1);
    const char* src = luaL_checkstring(L, 2);

    Assembler as{*state};
    if (as.run(addr, src))
    {
        state->changeset->blob(addr, (char*)as.code().data(), as.code().size() * 4);
    }
    else
    {
        lua_pushstring(L, "ASM error");
        lua_error(L);
    }

    return 0;
}

}

void API::initASM(lua_State* L, State* state)
{
    static struct luaL_Reg funcs[] = {
        { "patch",  &api_asm_patch },
        { nullptr }
    };

    lua_newtable(L);
    lua_pushlightuserdata(L, state);
    luaL_setfuncs(L, funcs, 1);
    lua_setglobal(L, "ASM");
}
