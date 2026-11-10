#include "shared/sdf/sdfDraw/decls.h"

void sdfDrawSync(void) {
    sdfDrawReqRec_t *table = sdfGrDrawState;

    for (;;) {
        SDF_FLAG_WAIT(&sdfDrawSyncFlag, sdfDrawSyncFlag != 0);
        if (table[sdfDrawReqQueueIndex].drawTag == NULL) {
            break;
        }
        if (table[sdfDrawReqQueueIndex].drawCount >= 2) {
            break;
        }
        sdfWaitVblank(0);
    }
}
