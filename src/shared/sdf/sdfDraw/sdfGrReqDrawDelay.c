#include "shared/sdf/sdfDraw/decls.h"

void sdfGrReqDrawDelay(s32 count) {
    SDF_FLAG_SET(&sdfReqDrawDelayCount, sdfReqDrawDelayCount,
                 (count < 0) ? 0 : count);
}
