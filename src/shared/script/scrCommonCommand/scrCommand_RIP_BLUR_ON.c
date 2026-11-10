#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_RIP_BLUR_ON(void)
{
    dds3RipBlurOn(scrGetIntPara(0));
    return 1;
}
