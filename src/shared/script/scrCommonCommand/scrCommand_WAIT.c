#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_WAIT(void)
{
    if (scrGetIntPara(0) <= 0)
    {
        return 1;
    }
    if (scrGetCommandTimer() < SDF_FRAMES(scrGetIntPara(0)))
    {
        return 0;
    }
    return 1;
}
