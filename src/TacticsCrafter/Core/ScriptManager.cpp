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
        /* Execute the script */
        auto& s = *ss.get();
        s.exec();
    }
    emit update();
}

