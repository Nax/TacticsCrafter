#include <TacticsCrafter/API/API.h>

void API::init(lua_State* L, State* state)
{
    initPatch(L, state);
    initScript(L, state);
    initSymbols(L, state);

    /* Misc */
    lua_newtable(L);
    lua_setglobal(L, "Registry");
}
