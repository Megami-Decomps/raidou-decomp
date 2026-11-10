#include "shared/script/scrCommonCommand/decls.h"

s32 scrCommand_SCR_TIME(void)
{
    dds3ProcessID_t *id;

    id = EE_ADDR_AS(dds3ProcessID_t *, scrGetIntPara(0));
    if (dds3ExistProcess(id) == 0)
    {
        scrSetIntReturnValue(0);
    }
    else
    {
        scrSetIntReturnValue(SDF_FRAME_TICK(dds3GetProcessRunTime(id)));
    }
    return 1;
}
