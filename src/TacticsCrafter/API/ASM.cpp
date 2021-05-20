#include <cstdint>
#include <TacticsCrafter/API/API.h>
#include <TacticsCrafter/Core/Assembler.h>

namespace
{

void asmError(lua_State* L, Assembler& as)
{
    luaL_Buffer b;

    luaL_buffinit(L, &b);
    luaL_addstring(&b, "ASM error line ");
    lua_pushinteger(L, as.line());
    luaL_addvalue(&b);
    luaL_addstring(&b, ": ");
    luaL_addstring(&b, as.error().c_str());
    luaL_pushresult(&b);

    lua_error(L);
}

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
        asmError(L, as);

    return 0;
}

int api_asm_new(lua_State* L)
{
    auto state = (State*)lua_touserdata(L, lua_upvalueindex(1));

    std::uint32_t addr = 0x08000000;
    const char* src = luaL_checkstring(L, 1);
    Assembler as{*state};

    if (!as.run(addr, src))
        goto error;

    lua_getglobal(L, "Memory");
    lua_getfield(L, -1, "alloc");
    lua_remove(L, -2);
    lua_pushinteger(L, as.code().size() * 4);
    lua_call(L, 1, 1);
    addr = (std::uint32_t)luaL_checkinteger(L, -1);

    if (!as.run(addr, src))
        goto error;

    state->changeset->blob(addr, (char*)as.code().data(), as.code().size() * 4);

    lua_pushinteger(L, addr);
    return 1;

error:
    asmError(L, as);
    return 0;
}

}

void API::initASM(lua_State* L, State* state)
{
    static struct luaL_Reg funcs[] = {
        { "patch",  &api_asm_patch },
        { "new",    &api_asm_new },
        { nullptr }
    };

    lua_newtable(L);
    lua_pushlightuserdata(L, state);
    luaL_setfuncs(L, funcs, 1);
    lua_setglobal(L, "ASM");
}
