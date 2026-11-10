#include "shared/sdf/sdfDraw/decls.h"

void sdfGrInitDraw(void) {
    /* Plain: runs before the draw thread exists, so no waiter. */
    sdfReqDrawDelayCount = 0;
    sdfGrDrawState[0].drawTag = NULL;
    sdfDrawReqQueueIndex = 0;
}
