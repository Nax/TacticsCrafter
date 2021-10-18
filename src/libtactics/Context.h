#ifndef LTC_CONTEXT_H
#define LTC_CONTEXT_H

#include <unordered_map>
#include <cstdint>
#include <string>

#include <libtactics/tactics.h>
#include <libtactics/Changeset.h>
#include <libtactics/Lua.h>
#include <libtactics/Option.h>
#include <libtactics/ResourcePool.h>
#include <libtactics/Script.h>

struct LTC_Context
{
    lua_State*  L;
    Script*     currentScript;
    Changeset   changeset;
    std::unordered_map<std::string, std::uint32_t>  symbols;
    std::uint32_t extraMemory;

    std::vector<LTC_Script> pipeline;

    ResourcePool<Script, LTC_Script> scripts;
    ResourcePool<Option, LTC_Option> options;
};

void ltcImplPipelineRun(LTC_Context* ctx);

#endif /* LTC_CONTEXT_H */
