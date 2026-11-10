#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_BLUR2_OFF(void)
{
    dds3Blur2Off(scrGetIntPara(0));
    return 1;
}
