#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_RIP_BLUR_SWAP(void)
{
    dds3RipBlurSwap(scrGetIntPara(0));
    return 1;
}
