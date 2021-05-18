#ifndef TC_CORE_STATE_H
#define TC_CORE_STATE_H

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <TacticsCrafter/Core/Changeset.h>
#include <TacticsCrafter/Core/Script.h>

struct State
{
    Script::Meta                                    scriptMeta;
    Changeset                                       changeset;
    std::unordered_map<std::string, std::uint32_t>  symbols;
};


#endif /* TC_CORE_STATE_H */
