#include <libtactics/Context.h>

LTC_API int ltcGetOptionCount(LTC_Context* ctx, LTC_Script script)
{
    Script* s = ctx->scripts.get(script);
    if (!s)
        return 0;
    return (int)s->options.size();
}

LTC_API LTC_Option ltcGetOptionHandle(LTC_Context* ctx, LTC_Script script, int index)
{
    Script* s = ctx->scripts.get(script);
    if (!s)
        return 0;
    if (index < 0 || index >= s->options.size())
        return 0;
    return s->options[index];
}

LTC_API LTC_OptionType ltcGetOptionType(LTC_Context* ctx, LTC_Option opt)
{
    Option* o = ctx->options.get(opt);
    if (!o)
        return LTC_OPTION_UNDEFINED;
    return o->type;
}

LTC_API const char* ltcGetOptionName(LTC_Context* ctx, LTC_Option opt)
{
    Option* o = ctx->options.get(opt);
    if (!o)
        return nullptr;
    return o->text.c_str();
}

LTC_API int ltcGetOptionValueBoolean(LTC_Context* ctx, LTC_Option opt)
{
    Option* o = ctx->options.get(opt);
    if (!o || o->type != LTC_OPTION_BOOLEAN)
        return 0;
    return o->b;
}

LTC_API void ltcSetOptionValueBoolean(LTC_Context* ctx, LTC_Option opt, int value)
{
    Option* o = ctx->options.get(opt);
    if (!o || o->type != LTC_OPTION_BOOLEAN)
        return;
    o->b = !!value;
}
