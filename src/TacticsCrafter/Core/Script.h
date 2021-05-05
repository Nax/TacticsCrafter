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
    const QString& version() const { return _version; }
    const QString& author() const { return _author; }
    const QString& description() const { return _description; }

    void exec();

private:
    lua_State*  _lua;
    int         _func;
    int         _env;

    QString     _name;
    QString     _version;
    QString     _author;
    QString     _description;
};

#endif /* TC_CORE_SCRIPT_H */
