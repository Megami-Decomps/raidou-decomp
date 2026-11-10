#include "shared/sdf/sdfGraph/decls.h"

void sdfDeliverDrawStart(s32 frameId, s32 buffId)
{
    if (sdfDispFrameId[0] == frameId) {
        sdfDispFrameId[0] = frameId ^ 1;
    }
    if (sdfDispFrameId[1] == frameId) {
        sdfDispFrameId[1] = frameId ^ 1;
    }
    SDF_FLAG_SET(&sdfGrDrawBusy, sdfGrDrawBusy, buffId);
}
