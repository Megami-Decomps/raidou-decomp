#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_BLUR2_ON(void)
{
    dds3Blur2On(scrGetIntPara(0));
    return 1;
}
