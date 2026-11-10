#include "shared/event/evtCommand/decls.h"

s32 evtCommand_CALL_ENDSAVE(void)
{
    dds3AdminiCallSequence(5, 0, 0, 0);
    return 1;
}
