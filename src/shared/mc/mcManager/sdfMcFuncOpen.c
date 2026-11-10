#include "shared/mc/mcManager/decls.h"

void sdfMcFuncOpen(u32 port, const char *path, s32 openFlags)
{
    sceMcOpen(port, MC_CARD_SLOT, path, openFlags);
}
