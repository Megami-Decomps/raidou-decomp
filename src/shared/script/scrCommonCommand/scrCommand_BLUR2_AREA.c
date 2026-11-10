#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_BLUR2_AREA(void)
{
    s32 size;

    size = scrGetIntPara(0);
    dds3SetBlur2Area(size, scrGetIntPara(1));
    return 1;
}
