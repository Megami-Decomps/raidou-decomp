#include "shared/mc/mcManager/decls.h"

void sdfMcFuncChdir(u32 port, const char *path)
{
    sceMcChdir(port, MC_CARD_SLOT, path, 0);
}
