#include <cstdint>
#include <libtactics/API/API.h>

namespace
{

int api_misc_print(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));
    Script* s = ctx->currentScript;
    int nargs = lua_gettop(L);

    for (int i = 1; i <= nargs; ++i)
    {
        luaL_tolstring(L, i, nullptr);
        s->log += lua_tostring(L, -1);
        lua_pop(L, -1);
    }
    s->log += "\n";

    return 0;
}

}

void API::initMisc(LTC_Context* ctx)
{
    lua_State* L = ctx->L;

    /* print redirection */
    lua_pushlightuserdata(L, ctx);
    lua_pushcclosure(L, &api_misc_print, 1);
    lua_setglobal(L, "print");

    /* Registry */
    /* TODO: change this */
    lua_newtable(ctx->L);
    lua_setglobal(ctx->L, "Registry");
}
