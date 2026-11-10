#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_RIP_BLUR_OFF(void)
{
    dds3RipBlurOff(scrGetIntPara(0));
    return 1;
}
