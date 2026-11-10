#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_FILTER2_OFF(void)
{
    dds3Filter2Off(scrGetIntPara(0));
    return 1;
}
