#include "shared/facility/fclMisc/decls.h"

s32 fclStartMessage(s32 MsgNo)
{
    if (gMsgID < 0)
    {
        return 0;
    }
    itfMesMngMarkBitStat(gMsgID, 0x200000);
    itfMesMngRequestMessage(gMsgID, MsgNo, 0);
    itfMesMngSetSelPhase(gMsgID, -1);
    gMsgFlag = 1;
    return 1;
}
