#include <TacticsCrafter/State/State.h>

State::State()
{
}

void State::apply(lua_State* L)
{
    patch.apply(L);
    script.apply(L);
    symbols.apply(L);

    /* Misc */
    lua_newtable(L);
    lua_setglobal(L, "Registry");
}
