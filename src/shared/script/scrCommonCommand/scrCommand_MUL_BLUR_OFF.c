#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_MUL_BLUR_OFF(void)
{
    dds3MultiBlurOff(scrGetIntPara(0));
    return 1;
}
