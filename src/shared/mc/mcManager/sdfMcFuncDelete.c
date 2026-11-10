#include "shared/mc/mcManager/decls.h"

void sdfMcFuncDelete(u32 port, const char *path)
{
    sceMcDelete(port, MC_CARD_SLOT, path);
}
