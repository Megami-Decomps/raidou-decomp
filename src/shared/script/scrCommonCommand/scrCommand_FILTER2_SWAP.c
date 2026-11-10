#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_FILTER2_SWAP(void)
{
    dds3Filter2Swap(scrGetIntPara(0));
    return 1;
}
