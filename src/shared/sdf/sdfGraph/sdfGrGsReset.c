#include "shared/sdf/sdfGraph/decls.h"

void sdfGrGsReset(void)
{
    s32 inter;
    s32 ffmode;

    switch (sdfGrFrameModes[sdfGrScreenMode].fieldFrameMode) {
    case 0:
        inter = 0;
        ffmode = 1;
        break;
    case 1:
        inter = 1;
        ffmode = 0;
        break;
    default:
        inter = 1;
        ffmode = 1;
        break;
    }
    sceGsResetGraph(sdfGrGsResetDone, inter, 2, ffmode);
    sdfGrGsResetDone = 1;
}
