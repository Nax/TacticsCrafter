#define WIN32_LEAN_AND_MEAN 1
#define VC_EXTRALEAN 1
#include <windows.h>
#include <string>

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
    return std::string(path) + "\\data";
}
