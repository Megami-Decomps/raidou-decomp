#include "shared/sdf/sdfSystem/decls.h"

void sdfAlarmWakeup(s32 arg0, s32 arg1, s32 thid)
{
    iWakeupThread(thid);
}
