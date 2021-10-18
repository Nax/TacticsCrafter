#include <libtactics/API/API.h>

void API::init(LTC_Context* ctx)
{
    initASM(ctx);
    initMemory(ctx);
    initPatch(ctx);
    initScript(ctx);
    initSymbols(ctx);

    /* Misc */
    initMisc(ctx);
}
