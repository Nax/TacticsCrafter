#ifndef LTC_SCRIPT_H
#define LTC_SCRIPT_H

#include <string>
#include <vector>
#include <libtactics/tactics.h>

struct Script
{
    std::vector<LTC_Option> options;

    std::string name;
    std::string author;
    std::string version;
    std::string description;

    std::string log;
    int         func;
    bool        core:1;
    bool        error:1;
};

#endif /* LTC_SCRIPT_H */
