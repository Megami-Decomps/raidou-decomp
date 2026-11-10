#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_BIT_CHK(void)
{
    scrSetIntReturnValue(evtBitCheck(scrGetIntPara(0)));
    return 1;
}
