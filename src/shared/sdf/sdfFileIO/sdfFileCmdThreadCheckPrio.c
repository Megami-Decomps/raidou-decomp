#include "shared/sdf/sdfFileIO/decls.h"

void sdfFileCmdThreadCheckPrio(void)
{
    if (sdfFileCmdThreadPrioCheckCounter != 0) {
        sdfFileCmdThreadPrioCheckCounter--;
        if (sdfFileCmdThreadPrioCheckCounter == 0) {
            sdfFileCmdThreadSetHiPrio();
        }
    }
}
