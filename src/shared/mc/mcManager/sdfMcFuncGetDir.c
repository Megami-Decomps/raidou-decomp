#include "shared/mc/mcManager/decls.h"

void sdfMcFuncGetDir(u32 port, const char *path, void *directoryEntries, s32 entryLimit)
{
    sceMcGetDir(port, MC_CARD_SLOT, path, 0, entryLimit, directoryEntries);
}
