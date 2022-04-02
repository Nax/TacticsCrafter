#include <libtactics/Path.h>

std::string ltcImplGetParentPath(const std::string& path)
{
    size_t lastSlash = path.rfind('/');
    if (lastSlash == std::string::npos)
        return path;
    std::string res = path.substr(0, lastSlash);
    if (res.find('/') == std::string::npos)
        res += '/';
    return res;
}
