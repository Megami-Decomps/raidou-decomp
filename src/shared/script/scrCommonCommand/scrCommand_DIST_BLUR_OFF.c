#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_DIST_BLUR_OFF(void)
{
    dds3DistBlurOff(scrGetIntPara(0));
    return 1;
}
