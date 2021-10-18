#include <libtactics/Context.h>

LTC_API const char* ltcGetScriptString(LTC_Context* ctx, LTC_Script script, LTC_ScriptInfo info)
{
    Script* s = ctx->scripts.get(script);

    if (!s)
        return nullptr;

    switch (info)
    {
    case LTC_SCRIPT_NAME:
        return s->name.c_str();
    case LTC_SCRIPT_AUTHOR:
        return s->author.c_str();
    case LTC_SCRIPT_VERSION:
        return s->version.c_str();
    case LTC_SCRIPT_DESCRIPTION:
        return s->description.c_str();
    case LTC_SCRIPT_LOG:
        return s->log.c_str();
    }

    return nullptr;
}

LTC_API int ltcGetScriptInt(LTC_Context* ctx, LTC_Script script, LTC_ScriptInfo info)
{
    Script* s = ctx->scripts.get(script);

    if (!s)
        return -1;

    switch (info)
    {
    case LTC_SCRIPT_ERROR:
        return (int)s->error;
    case LTC_SCRIPT_CORE:
        return (int)s->core;
    }

    return -1;
}
