#ifndef TC_CORE_SCRIPT_H
#define TC_CORE_SCRIPT_H

#include <QStringList>
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

    Script(lua_State* lua, const QString& path, bool core = false);
    ~Script();

    bool core() const { return _core; }

    const QStringList&  log() const { return _log; }
    void                print(const char* str) { _log.append(str); }

    const Meta& meta() const { return _meta; }
    void setMeta(const Meta& m) { _meta = m; }

    void exec();

private:
    lua_State*      _lua;
    bool            _core;
    int             _func;
    Meta            _meta;
    QStringList     _log;
};

#endif /* TC_CORE_SCRIPT_H */
