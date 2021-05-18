#ifndef TC_STATE_STATE_SYMBOLS_H
#define TC_STATE_STATE_SYMBOLS_H

#include <string>
#include <unordered_map>
#include <TacticsCrafter/Core/Lua.h>

class StateSymbols
{
public:
    StateSymbols();
    void apply(lua_State* L);

    std::unordered_map<std::string, std::uint32_t> syms;
};

#endif /* TC_STATE_STATE_SYMBOLS_H */
