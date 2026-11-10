#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_BIT_OFF(void)
{
    evtBitOff(scrGetIntPara(0));
    return 1;
}
