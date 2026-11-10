#include "shared/event/evtCommand/decls.h"

s32 evtCommand_MOON_START(void)
{
    evtEnableMoonCalc();
    return 1;
}
