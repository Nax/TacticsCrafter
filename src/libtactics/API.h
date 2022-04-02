#ifndef LTC_API_H
#define LTC_API_H

#include <libtactics/Context.h>

namespace API
{

void init(LTC_Context* ctx);
void initASM(LTC_Context* ctx);
void initMemory(LTC_Context* ctx);
void initPatch(LTC_Context* ctx);
void initScript(LTC_Context* ctx);
void initSymbols(LTC_Context* ctx);

void initMisc(LTC_Context* ctx);

}

#endif /* TC_API_H */
