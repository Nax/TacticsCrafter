#ifndef TC_STATE_STATE_H
#define TC_STATE_STATE_H

#include <TacticsCrafter/State/StateSymbols.h>

struct State
{
    State();
    void apply(lua_State* L);

    StateSymbols symbols;
};

#endif /* TC_STATE_STATE_H */
