#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_DIST_BLUR_ON(void)
{
    dds3DistBlurOn(scrGetIntPara(0));
    return 1;
}
