#include <QCoreApplication>
#include <QDir>
#include <TacticsCrafter/Core/ScriptManager.h>
#include <TacticsCrafter/State/State.h>

ScriptManager::ScriptManager()
{
    _lua = luaL_newstate();
    luaL_openlibs(_lua);

    QString dirPath = QCoreApplication::applicationDirPath() + "/data/core/";
    QDir coreDir(dirPath);
    auto coreScripts = coreDir.entryList(QStringList("*.lua"), QDir::Filter::Files, QDir::SortFlag::Name);
    for (const auto& s : coreScripts)
    {
        load(dirPath + s, true);
    }
    run();
}

ScriptManager::~ScriptManager()
{
    lua_close(_lua);
}

void ScriptManager::load(const QString& path, bool core)
{
    _scripts.push_back(std::make_unique<Script>(_lua, path, core));
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
