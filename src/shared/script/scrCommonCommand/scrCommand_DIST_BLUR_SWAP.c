#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_DIST_BLUR_SWAP(void)
{
    dds3DistBlurSwap(scrGetIntPara(0));
    return 1;
}
