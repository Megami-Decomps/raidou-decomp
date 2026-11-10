#include "shared/sdf/sdfFileIO/decls.h"

void sdfFileCmdThreadSetHiPrio(void)
{
    sdfFileCmdThreadSetPrio(0x48);
}
