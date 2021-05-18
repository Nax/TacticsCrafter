#ifndef TC_CORE_SCRIPT_H
#define TC_CORE_SCRIPT_H

#include <QString>
#include <TacticsCrafter/Core/Lua.h>

class Script
{
public:
    struct Meta
    {
        QString name;
        QString version;
        QString author;
        QString description;
    };

    Script(lua_State* lua, const QString& path);
    ~Script();

    const Meta& meta() const { return _meta; }
    void setMeta(const Meta& m) { _meta = m; }

    void exec();

private:
    lua_State*  _lua;
    int         _func;
    Meta        _meta;
};

#endif /* TC_CORE_SCRIPT_H */
