#ifndef LIBTACTICS_TACTICS_H
#define LIBTACTICS_TACTICS_H

#if defined(__cplusplus)
# define LTC_EXTERN extern "C"
#else
# define LTC_EXTERN
#endif

#if defined(WIN32) || defined(_WIN32)
# if defined(LIBTACTICS_EXPORT)
#  define LTC_ABI __declspec(dllexport)
# else
#  define LTC_ABI __declspec(dllimport)
# endif
#elif (defined(__GNUC__) || defined(__clang__)) && defined(LIBTACTICS_EXPORT)
# define LTC_ABI __attribute__((visibility("default")))
#else
# define LTC_ABI
#endif

#define LTC_API LTC_EXTERN LTC_ABI

#include <stdint.h>
#include <stddef.h>

typedef struct LTC_Context LTC_Context;

typedef uint32_t LTC_Script;
typedef uint32_t LTC_Option;

enum LTC_ScriptInfo
{
    LTC_SCRIPT_NAME = 1,
    LTC_SCRIPT_AUTHOR = 2,
    LTC_SCRIPT_DESCRIPTION = 3,
    LTC_SCRIPT_VERSION = 4,
    LTC_SCRIPT_LOG = 5,
    LTC_SCRIPT_ERROR = 6,
    LTC_SCRIPT_CORE = 7,
};

enum LTC_OptionType
{
    LTC_OPTION_UNDEFINED = 0,
    LTC_OPTION_BOOLEAN = 1
};

typedef void (*LTC_ProgressCallback)(void*, int);

/* Context */
LTC_API LTC_Context*    ltcCreateContext(const char* dataPath, const char* projectFile);
LTC_API void            ltcDestroyContext(LTC_Context* ctx);
LTC_API LTC_Script      ltcLoadScript(LTC_Context* ctx, const char* path);
LTC_API void            ltcRemoveScript(LTC_Context* ctx, LTC_Script script);
LTC_API void            ltcMoveScript(LTC_Context* ctx, LTC_Script script, int delta);
LTC_API int             ltcGetScriptCount(LTC_Context* ctx);
LTC_API LTC_Script      ltcGetScriptHandle(LTC_Context* ctx, int index);
LTC_API void            ltcRunPipeline(LTC_Context* ctx);
LTC_API void            ltcSaveContext(LTC_Context* ctx, const char* path);

/* Script */
LTC_API const char* ltcGetScriptString(LTC_Context* ctx, LTC_Script script, LTC_ScriptInfo info);
LTC_API int         ltcGetScriptInt(LTC_Context* ctx, LTC_Script script, LTC_ScriptInfo info);

/* ImageBuilder */
LTC_API void ltcBuildImage(LTC_Context* ctx, const char* dst, const char* src, LTC_ProgressCallback cb, void* arg);

/* Option */
LTC_API int             ltcGetOptionCount(LTC_Context* ctx, LTC_Script script);
LTC_API LTC_Option      ltcGetOptionHandle(LTC_Context* ctx, LTC_Script script, int index);
LTC_API LTC_OptionType  ltcGetOptionType(LTC_Context* ctx, LTC_Option opt);
LTC_API const char*     ltcGetOptionName(LTC_Context* ctx, LTC_Option opt);
LTC_API int             ltcGetOptionValueBoolean(LTC_Context* ctx, LTC_Option opt);
LTC_API void            ltcSetOptionValueBoolean(LTC_Context* ctx, LTC_Option opt, int value);

#endif /* LIBTACTICS_TACTICS_H */
