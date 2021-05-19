#ifndef TC_API_H
#define TC_API_H

#include <TacticsCrafter/Core/Lua.h>
#include <TacticsCrafter/Core/State.h>

namespace API
{

void init(lua_State* L, State* state);
void initASM(lua_State* L, State* state);
void initPatch(lua_State* L, State* state);
void initScript(lua_State* L, State* state);
void initSymbols(lua_State* L, State* state);

}

#endif /* TC_API_H */
