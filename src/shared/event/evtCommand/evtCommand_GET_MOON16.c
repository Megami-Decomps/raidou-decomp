#include "shared/event/evtCommand/decls.h"

s32 evtCommand_GET_MOON16(void)
{
    scrSetIntReturnValue(evtGetAgeOfMoon16());
    return 1;
}
