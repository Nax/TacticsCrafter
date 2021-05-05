#ifndef TC_CORE_SCRIPT_H
#define TC_CORE_SCRIPT_H

#include <QString>
#include <TacticsCrafter/Core/Lua.h>

class Script
{
public:
    Script(lua_State* lua, const QString& path);
    ~Script();

    const QString& name() const { return _name; }

    void setName(const QString& name);
    void exec();

private:
    lua_State*  _lua;
    int         _func;
    QString     _name;
};

#endif /* TC_CORE_SCRIPT_H */
