#include "common.h"
#include "sdk_libmc.h"
#include "mcManager.h"

void sdfMcFuncGetInfo(s32 port)
{
    sdfMcCb_tag *cb = &sdfMcCb[port];

    sceMcGetInfo(port, MC_CARD_SLOT, &cb->type, &cb->free, &cb->format);
}
