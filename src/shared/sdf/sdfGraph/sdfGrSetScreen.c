#include "shared/sdf/sdfGraph/decls.h"

void sdfGrSetScreen(s32 aMode)
{
    sdfGrScreenMode = aMode;
    sdfGrSetFrameSetMode(&sdfGrFrameSet, aMode, 0);
    reqMakeFrameBuff = 1;
}
