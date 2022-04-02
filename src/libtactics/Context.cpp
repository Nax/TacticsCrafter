#include <dirent.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <json/json.h>
#include <libtactics/Context.h>
#include <libtactics/API.h>
#include <libtactics/Path.h>

#if defined(WIN32) || defined(_WIN32)
# include <direct.h>
# define chdir _chdir
#else
# include <unistd.h>
#endif

namespace
{

std::string makeRelativePath(const std::string& root, const std::string& path)
{
    if (path.size() > root.size() && path.find(root) == 0)
        return path.substr(root.size(), path.size() - root.size());
    return path;
}

}

void ltcImplPipelineRun(LTC_Context* ctx)
{
    lua_State* L = ctx->L;
    bool error{};

    /* Reset our state */
    ctx->changeset.clear();
    ctx->symbols.clear();
    ctx->extraMemory = 0;

    /* Create a new lua ENV */
    lua_newtable(L);
    lua_newtable(L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);

    for (LTC_Script script : ctx->pipeline)
    {
        /* Flag the script as current for the lua API */
        Script* s = ctx->scripts.get(script);
        ctx->currentScript = s;

        if (s->func == LUA_NOREF)
        {
            error = true;
        }
        else
        {
            /* Reset the script metadata */
            s->resetMetadata();

            /* Load the script function */
            lua_rawgeti(L, LUA_REGISTRYINDEX, s->func);

            /* Set the env */
            lua_pushvalue(L, 1);
            lua_setupvalue(L, -2, 1);

            /* Call the func */
            s->error = false;
            s->log = "";
            if (lua_pcall(L, 0, 0, 0))
            {
                s->error = true;
                s->log = lua_tostring(L, -1);
                lua_pop(L, -1);
                error = true;
            }
        }

        if (error)
            break;
    }

    /* Remove the env */
    lua_pop(L, 1);
}

static LTC_Script ltcImplPipelineLoadScript(LTC_Context* ctx, const char* path, bool core)
{
    lua_State* L = ctx->L;
    LTC_Script script;
    Script* s;

    /* Allow local scripts */
    if (!core && !ctx->dir.empty())
        chdir(ctx->dir.c_str());

    /* Alloc the script */
    script = ctx->scripts.alloc();
    s = ctx->scripts.get(script);

    /* Register the path */
    s->path = ltcImplGetAbsolutePath(path);

    /* Create the script function */
    if (luaL_loadfile(L, path))
    {
        s->func = LUA_NOREF;
        s->log = lua_tostring(L, -1);
        lua_pop(L, -1);
        s->error = true;
    }
    else
    {
        s->func = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    s->core = core;
    ctx->pipeline.push_back(script);

    return script;
}

LTC_API void ltcRemoveScript(LTC_Context* ctx, LTC_Script script)
{
    lua_State* L = ctx->L;
    Script* s = ctx->scripts.get(script);

    if (!s || s->core)
        return;

    /* Remove options */
    for (auto o : s->options)
        ctx->options.dealloc(o);

    /* Remove the lua handle */
    if (s->func != LUA_NOREF)
        luaL_unref(L, LUA_REGISTRYINDEX, s->func);

    /* Remove the script from the pipeline */
    auto it = std::find(ctx->pipeline.begin(), ctx->pipeline.end(), script);
    if (it != ctx->pipeline.end())
        ctx->pipeline.erase(it);

    /* Remove the script itself */
    ctx->scripts.dealloc(script);
}

LTC_API void ltcMoveScript(LTC_Context* ctx, LTC_Script script, int delta)
{
    lua_State* L = ctx->L;
    Script* s = ctx->scripts.get(script);

    if (!s || s->core)
        return;

    int index{};
    for (index = 0; index < ctx->pipeline.size(); ++index)
    {
        if (ctx->pipeline[index] == script)
            break;
    }
    int index2 = index + delta;
    if (index2 < 0 || index2 >= ctx->pipeline.size())
        return;
    Script* s2 = ctx->scripts.get(ctx->pipeline[index2]);
    if (!s2 || s2->core)
        return;

    ctx->pipeline[index] = ctx->pipeline[index2];
    ctx->pipeline[index2] = script;
}

LTC_API LTC_Context* ltcCreateContext(const char* projectFile)
{
    auto dataPath = ltcImplGetDataPath();

    LTC_Context* ctx;
    lua_State* L;
    luaL_Buffer b;

    ctx = new LTC_Context{};

    /* Create the lua state */
    L = luaL_newstate();
    luaL_openlibs(L);

    /* Set the search path */
    lua_getglobal(L, "package");
    luaL_buffinit(L, &b);
    luaL_addstring(&b, dataPath.c_str());
    luaL_addstring(&b, "/?.lua");
    luaL_pushresult(&b);
    lua_setfield(L, -2, "path");
    lua_pop(L, 1);

    /* Set the lua state */
    ctx->L = L;

    /* Load our API */
    API::init(ctx);

    /* Load core scripts */
    std::vector<std::string> coreScripts;
    std::string corePath = dataPath + "/core/";
    DIR* coreDir = opendir(corePath.c_str());
    dirent* ent;
    while ((ent = readdir(coreDir)))
    {
        const char* ext = strrchr(ent->d_name, '.');
        if (ext && strcmp(ext, ".lua") == 0)
        {
            coreScripts.push_back(corePath + "/" + ent->d_name);
        }
    }
    closedir(coreDir);

    std::sort(coreScripts.begin(), coreScripts.end());
    for (auto& e : coreScripts)
    {
        ltcImplPipelineLoadScript(ctx, e.c_str(), true);
    }

    /* Load the project file */
    if (projectFile)
    {
        ctx->dir = ltcImplGetParentPath(ltcImplGetAbsolutePath(projectFile));

        Json::Value root;

        {
            std::ifstream f{projectFile};
            if (!f.good())
                goto loaded;
            try {
                f >> root;
            } catch (const std::exception&) {
                goto loaded;
            }
        }

        Json::Value scripts = root["scripts"];
        for (Json::Value v : scripts)
        {
            Json::Value p = v["path"];
            LTC_Script scriptID = ltcImplPipelineLoadScript(ctx, p.asCString(), false);
            Script* s = ctx->scripts.get(scriptID);
            Json::Value options = v["options"];
            if (options.isObject())
            {
                for (const auto& key : options.getMemberNames())
                {
                    const auto& value = options[key];
                    LTC_Option optionID = ctx->options.alloc();
                    Option* o = ctx->options.get(optionID);
                    o->text = "";
                    o->key = key;
                    if (value.isBool())
                    {
                        o->type = LTC_OPTION_BOOLEAN;
                        o->b = value.asBool();
                    }
                    else
                    {
                        o->type = LTC_OPTION_UNDEFINED;
                    }
                    s->options.push_back(optionID);
                }
            }
        }
    }
loaded:

    /* Run the pipeline once to process the core scripts */
    ltcImplPipelineRun(ctx);

    return ctx;
}

LTC_API void ltcDestroyContext(LTC_Context* ctx)
{
    lua_close(ctx->L);
    delete ctx;
}

LTC_API LTC_Script ltcLoadScript(LTC_Context* ctx, const char* path)
{
    return ltcImplPipelineLoadScript(ctx, path, false);
}

LTC_API int ltcGetScriptCount(LTC_Context* ctx)
{
    return (int)ctx->pipeline.size();
}

LTC_API LTC_Script ltcGetScriptHandle(LTC_Context* ctx, int index)
{
    if (index < 0 || index >= ctx->pipeline.size())
        return 0;
    return ctx->pipeline[index];
}

LTC_API void ltcRunPipeline(LTC_Context* ctx)
{
    ltcImplPipelineRun(ctx);
}

LTC_API void ltcSaveContext(LTC_Context* ctx, const char* path)
{
    Json::Value root(Json::objectValue);
    Json::Value scripts(Json::arrayValue);

    ctx->dir = ltcImplGetParentPath(ltcImplGetAbsolutePath(path));
    std::string localPrefix = ctx->dir + '/';

    for (LTC_Script script : ctx->pipeline)
    {
        Script* s = ctx->scripts.get(script);
        if (!s || s->core)
            continue;
        Json::Value scriptObj(Json::objectValue);
        scriptObj["path"] = makeRelativePath(localPrefix, s->path).c_str();

        if (!s->options.empty())
        {
            Json::Value optionsObj(Json::objectValue);
            for (auto optionId : s->options)
            {
                const Option* o = ctx->options.get(optionId);
                optionsObj[o->key] = o->b;
            }
            scriptObj["options"] = optionsObj;
        }
        scripts.append(scriptObj);
    }

    root["scripts"] = scripts;

    /* Write the file */
    std::ofstream f(path);
    f << root << std::endl;
    f.close();
}
