#include "shared/mc/mcManager/decls.h"

void sdfMcFuncMkdir(u32 mccb, const char *cb)
{
    sceMcMkdir(mccb, MC_CARD_SLOT, cb);
}
