#include <libtactics/API/API.h>

namespace
{

int api_script_properties(lua_State* L)
{
    const char* tmp;

    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));
    lua_getfield(L, 1, "name");
    if ((tmp = luaL_checkstring(L, -1)))
        ctx->currentScript->name = tmp;
    lua_pop(L, 1);
    lua_getfield(L, 1, "version");
    if ((tmp = luaL_checkstring(L, -1)))
        ctx->currentScript->version = tmp;
    lua_pop(L, 1);
    lua_getfield(L, 1, "author");
    if ((tmp = luaL_checkstring(L, -1)))
        ctx->currentScript->author = tmp;
    lua_pop(L, 1);
    lua_getfield(L, 1, "description");
    if ((tmp = luaL_checkstring(L, -1)))
        ctx->currentScript->description = tmp;
    lua_pop(L, 1);

    return 0;
}

int api_script_opt_bool(lua_State* L)
{
    LTC_Context* ctx = (LTC_Context*)lua_touserdata(L, lua_upvalueindex(1));

    const char* key = luaL_checkstring(L, 1);
    const char* text = luaL_checkstring(L, 2);
    bool value = lua_isnone(L, 3) ? false : lua_toboolean(L, 3);

    /* Search for the option */
    Script* s = ctx->currentScript;
    Option* optFound{};

    for (auto opt : s->options)
    {
        Option* o = ctx->options.get(opt);
        if (!o || o->key != key)
            continue;
        optFound = o;
        break;
    }
    if (!optFound)
    {
        auto opt = ctx->options.alloc();
        optFound = ctx->options.get(opt);
        s->options.push_back(opt);
        optFound->type = LTC_OPTION_UNDEFINED;
        optFound->key = key;
    }
    if (optFound->type != LTC_OPTION_BOOLEAN)
    {
        optFound->type = LTC_OPTION_BOOLEAN;
        optFound->b = value;
    }
    optFound->text = text;
    lua_pushboolean(L, optFound->b);

    return 1;
}

}

void API::initScript(LTC_Context* ctx)
{
    static struct luaL_Reg funcs[] = {
        { "properties", &api_script_properties },
        { "opt_bool", &api_script_opt_bool },
        { nullptr }
    };

    lua_State* L = ctx->L;

    lua_newtable(L);
    lua_pushlightuserdata(L, ctx);
    luaL_setfuncs(L, funcs, 1);
    lua_setglobal(L, "Script");
}
