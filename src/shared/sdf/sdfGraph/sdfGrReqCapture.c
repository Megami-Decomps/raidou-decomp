#include "shared/sdf/sdfGraph/decls.h"

void sdfGrReqCapture(s32 aBuff, s32 aCallback)
{
    sdfGrCaptureReq = aBuff;
    sdfGrCaptureTarget = aCallback;
    sdfGrCaptureBuff = 1;
}
