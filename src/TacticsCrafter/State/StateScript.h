#ifndef TC_STATE_STATE_SCRIPT_H
#define TC_STATE_STATE_SCRIPT_H

#include <string>
#include <unordered_map>
#include <TacticsCrafter/Core/Lua.h>
#include <TacticsCrafter/Core/Script.h>

class StateScript
{
public:
    StateScript();

    void reset();
    void apply(lua_State* L);

    Script::Meta meta;
};

#endif /* TC_STATE_STATE_SCRIPT_H */
