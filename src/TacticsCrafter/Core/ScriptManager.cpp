#include <QCoreApplication>
#include <TacticsCrafter/Core/ScriptManager.h>
#include <TacticsCrafter/State/State.h>

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

Changeset ScriptManager::run()
{
    State state;

    state.apply(_lua);

    for (auto& ss : _scripts)
    {
        /* Reset the script metadata (name, author, etc.) */
        state.script.reset();

        /* Execute the script */
        auto& s = *ss.get();
        s.exec();

        /* Store the new metadata */
        s.setMeta(state.script.meta);
    }
    emit update();

    return state.patch.changeset;
}
