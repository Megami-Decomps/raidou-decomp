#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_BLUR2_SWAP(void)
{
    dds3Blur2Swap(scrGetIntPara(0));
    return 1;
}
