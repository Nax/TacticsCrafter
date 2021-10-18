#ifndef LTC_OPTION_H
#define LTC_OPTION_H

#include <string>

struct Option
{
    LTC_OptionType type;
    std::string text;
    std::string key;
    union
    {
        bool b;
    };
};

#endif /* LTC_OPTION_H */
