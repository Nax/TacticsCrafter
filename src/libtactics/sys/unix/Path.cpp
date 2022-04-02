#include <string>
#include <libtactics/Path.h>

std::string ltcImplGetAbsolutePath(const std::string& path)
{
    char* s = realpath(path.c_str(), nullptr);
    std::string abs{s};
    std::free(s);
    return s;
}
