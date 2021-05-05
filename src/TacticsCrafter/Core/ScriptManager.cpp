#include <TacticsCrafter/Core/ScriptManager.h>

ScriptManager::ScriptManager()
{
    _lua = luaL_newstate();
    luaL_openlibs(_lua);
}

ScriptManager::~ScriptManager()
{
    lua_close(_lua);
}

void ScriptManager::load(const QString& path)
{
    _scripts.push_back(std::make_unique<Script>(_lua, path));
}
