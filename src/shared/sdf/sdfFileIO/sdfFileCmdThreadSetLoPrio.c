#include "shared/sdf/sdfFileIO/decls.h"

void sdfFileCmdThreadSetLoPrio(void)
{
    sdfFileCmdThreadPrioCheckCounter = 3;
    sdfFileCmdThreadSetPrio(0x78);
}
