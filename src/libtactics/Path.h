#ifndef LTC_PATH_H
#define LTC_PATH_H

#include <string>

std::string ltcImplGetDataPath(void);
std::string ltcImplGetAbsolutePath(const std::string& path);
std::string ltcImplGetParentPath(const std::string& path);

#endif

