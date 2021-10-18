#ifndef LTC_STATE_H
#define LTC_STATE_H

#include <cstdint>
#include <unordered_map>

struct State
{
    std::unordered_map<std::string, std::uint32_t>  symbols;
};

#endif /* LTC_STATE_H */
