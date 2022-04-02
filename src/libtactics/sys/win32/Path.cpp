#define WIN32_LEAN_AND_MEAN 1
#define VC_EXTRALEAN 1
#include <windows.h>
#include <string>
#include <libtactics/Path.h>

namespace
{
int sDummy;
}

std::string ltcImplGetDataPath(void)
{
    char path[4096];
    HMODULE mod;
    std::size_t len;

    GetModuleHandleExA(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCSTR)&sDummy,
        &mod
    );
    GetModuleFileNameA(mod, path, sizeof(path));
    len = std::strlen(path);
    for (std::size_t i = len - 1; i > 0; --i)
    {
        if (path[i] == '/' || path[i] == '\\')
        {
            path[i] = 0;
            break;
        }
    }
    return ltcImplGetAbsolutePath(path) + "/data";
}

std::string ltcImplGetAbsolutePath(const std::string& path)
{
    char* s = _fullpath(nullptr, path.c_str(), 0);
    std::string abs{s};
    std::free(s);
    for (std::size_t i = 0; i < abs.size(); ++i)
    {
        if (abs[i] == '\\')
            abs[i] = '/';
    }
    return abs;
}
