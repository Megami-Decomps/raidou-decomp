#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_RIP_BLUR_AREA(void)
{
    s32 size;
    s32 x;

    size = scrGetIntPara(0);
    x = scrGetIntPara(1);
    dds3SetRipBlurArea(size, x, scrGetIntPara(2));
    return 1;
}
