#include <QCoreApplication>
#include <TacticsCrafter/Core/ScriptManager.h>

ScriptManager::ScriptManager()
{
    _lua = luaL_newstate();
    luaL_openlibs(_lua);

    luaL_loadfile(_lua, (QCoreApplication::applicationDirPath() + "/data/api.lua").toStdString().c_str());
    lua_pcall(_lua, 0, 0, 0);
}

ScriptManager::~ScriptManager()
{
    lua_close(_lua);
}

void ScriptManager::load(const QString& path)
{
    _scripts.push_back(std::make_unique<Script>(_lua, path));
}

void ScriptManager::prerun()
{
    for (auto& ss : _scripts)
    {
        /* Prepare the script env */
        lua_newtable(_lua);
        lua_pushstring(_lua, "name");
        lua_pushstring(_lua, "Unknown");
        lua_settable(_lua, -3);
        lua_pushstring(_lua, "description");
        lua_pushstring(_lua, "Unknown");
        lua_settable(_lua, -3);
        lua_setglobal(_lua, "_script");

        /* Execute the script */
        auto& s = *ss.get();
        s.exec();

        /* Fetch the results */
        lua_getglobal(_lua, "_script");
        lua_pushstring(_lua, "name");
        lua_gettable(_lua, -2);
        auto* name = luaL_checkstring(_lua, -1);
        s.setName(name);
        lua_pop(_lua, 2);
    }
}

