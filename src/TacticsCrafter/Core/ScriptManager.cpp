#include <QCoreApplication>
#include <QDir>
#include <TacticsCrafter/Core/ScriptManager.h>
#include <TacticsCrafter/Core/State.h>
#include <TacticsCrafter/API/API.h>

ScriptManager::ScriptManager()
{
    /* Create the lua state */
    _lua = luaL_newstate();
    luaL_openlibs(_lua);

    /* Set the search path */
    QString luaPath = QCoreApplication::applicationDirPath() + "/data/?.lua";
    lua_getglobal(_lua, "package");
    lua_pushstring(_lua, luaPath.toStdString().c_str());
    lua_setfield(_lua, -2, "path");
    lua_pop(_lua, 1);

    /* Load core scripts */
    QString dirPath = QCoreApplication::applicationDirPath() + "/data/core/";
    QDir coreDir(dirPath);
    auto coreScripts = coreDir.entryList(QStringList("*.lua"), QDir::Filter::Files, QDir::SortFlag::Name);
    for (const auto& s : coreScripts)
    {
        load(dirPath + s, true);
    }

    /* Run the pipeline once to process the core scripts */
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

bool ScriptManager::moveUp(std::size_t index)
{
    if (index == 0)
        return false;

    std::size_t index2 = index - 1;

    if (_scripts[index]->core() || _scripts[index2]->core())
        return false;

    std::swap(_scripts[index], _scripts[index2]);
    run();
    return true;
}

bool ScriptManager::moveDown(std::size_t index)
{
    if (index == _scripts.size() - 1)
        return false;

    std::size_t index2 = index + 1;

    if (_scripts[index]->core() || _scripts[index2]->core())
        return false;

    std::swap(_scripts[index], _scripts[index2]);
    run();
    return true;
}

bool ScriptManager::remove(std::size_t index)
{
    if (_scripts[index]->core())
        return false;
    _scripts.erase(_scripts.begin() + index);
    run();
    return true;
}

const State& ScriptManager::run()
{
    _state = State();
    _changes.clear();
    _state.changeset = &_changes;
    API::init(_lua, &_state);

    for (auto& ss : _scripts)
    {
        /* Reset the script metadata (name, author, etc.) */
        _state.scriptMeta.name = "Unknown";
        _state.scriptMeta.author = "Unknown";
        _state.scriptMeta.version = "0.0.0";
        _state.scriptMeta.description = "N/A";

        /* Execute the script */
        auto& s = *ss.get();
        _state.script = &s;
        s.exec();

        /* Store the new metadata */
        s.setMeta(_state.scriptMeta);
    }
    emit update();

    return _state;
}
