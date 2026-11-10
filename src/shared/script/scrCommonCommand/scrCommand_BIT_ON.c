#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_BIT_ON(void)
{
    evtBitOn(scrGetIntPara(0));
    return 1;
}
