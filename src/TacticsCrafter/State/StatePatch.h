#ifndef TC_STATE_STATE_PATCH_H
#define TC_STATE_STATE_PATCH_H

#include <TacticsCrafter/Core/Lua.h>
#include <TacticsCrafter/Core/Changeset.h>

class StatePatch
{
public:
    StatePatch();

    void apply(lua_State* L);

    Changeset changeset;
};

#endif /* TC_STATE_STATE_PATCH_H */
