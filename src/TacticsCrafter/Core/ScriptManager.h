#ifndef TC_SCRIPT_MANAGER_H
#define TC_SCRIPT_MANAGER_H

#include <vector>
#include <memory>
#include <TacticsCrafter/Core/Lua.h>
#include <TacticsCrafter/Core/Script.h>

class ScriptManager
{
public:
    ScriptManager();
    ~ScriptManager();

    std::size_t count() const { return _scripts.size(); }

    const Script& get(std::size_t index) const { return *_scripts[index]; }
    Script& get(std::size_t index) { return *_scripts[index]; }

    void load(const QString& path);
    void prerun();

private:
    lua_State*  _lua;

    std::vector<std::unique_ptr<Script>>    _scripts;
};

#endif
