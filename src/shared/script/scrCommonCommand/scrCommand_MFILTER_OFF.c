#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_MFILTER_OFF(void)
{
    dds3MFilterOff(scrGetIntPara(0));
    return 1;
}
