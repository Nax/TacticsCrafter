#ifndef TC_CORE_SCRIPT_H
#define TC_CORE_SCRIPT_H

#include <vector>
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

    struct Option
    {
        enum class Type
        {
            Bool,
        };

        Type type;
        QString text;
        QString key;
        union
        {
            bool b;
        };
    };

    Script(lua_State* lua, const QString& path, bool core = false);
    ~Script();

    bool core() const { return _core; }

    auto&               opts() { return _opts; }
    const QStringList&  log() const { return _log; }
    void                print(const char* str) { _log.append(str); }

    const Meta& meta() const { return _meta; }
    void setMeta(const Meta& m) { _meta = m; }

    bool optBool(const QString& key, const QString& text, bool value = false);

    void exec();

private:
    Option* optLookup(const QString& key, Option::Type type);

    lua_State*              _lua;
    bool                    _core;
    int                     _func;
    Meta                    _meta;
    QStringList             _log;
    std::vector<Option>     _opts;
};

#endif /* TC_CORE_SCRIPT_H */
