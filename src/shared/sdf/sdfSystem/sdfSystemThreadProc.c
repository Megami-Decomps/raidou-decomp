#include "shared/sdf/sdfSystem/decls.h"

void sdfSystemThreadProc(void) {
    for (;;) {
        sdfWaitVblank(1);
        sdfPadManager();
        if (!sdfPadManagerMode) {
            sdfPadWork();
        }
        sdfFileCmdThreadCheckPrio();
        if (D_00551828 != NULL) {
            D_00551828();
        }
    }
}
