#ifndef TC_SCRIPT_MANAGER_H
#define TC_SCRIPT_MANAGER_H

#include <vector>
#include <memory>
#include <QObject>
#include <TacticsCrafter/Core/Lua.h>
#include <TacticsCrafter/Core/Script.h>
#include <TacticsCrafter/Core/State.h>
#include <TacticsCrafter/Core/Changeset.h>

class ScriptManager : public QObject
{
    Q_OBJECT

public:
    ScriptManager();
    ~ScriptManager();

    std::size_t count() const { return _scripts.size(); }

    const Script& get(std::size_t index) const { return *_scripts[index]; }
    Script& get(std::size_t index) { return *_scripts[index]; }

    void load(const QString& path, bool core = false);

    bool moveUp(std::size_t index);
    bool moveDown(std::size_t index);
    bool remove(std::size_t index);

    const State& run();

signals:
    void update();

private:
    lua_State*  _lua;

    State                                   _state;
    Changeset                               _changes;
    std::vector<std::unique_ptr<Script>>    _scripts;
};

#endif
