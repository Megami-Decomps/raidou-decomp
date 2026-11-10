#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_MFILTER_SWAP(void)
{
    dds3MFilterSwap(scrGetIntPara(0));
    return 1;
}
