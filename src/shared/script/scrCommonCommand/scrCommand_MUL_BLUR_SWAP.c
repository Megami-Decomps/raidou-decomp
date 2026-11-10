#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_MUL_BLUR_SWAP(void)
{
    dds3MultiBlurSwap(scrGetIntPara(0));
    return 1;
}
