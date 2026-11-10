#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_MUL_BLUR_ON(void)
{
    dds3MultiBlurOn(scrGetIntPara(0));
    return 1;
}
