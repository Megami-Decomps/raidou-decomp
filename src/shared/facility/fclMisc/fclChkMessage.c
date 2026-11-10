#include "shared/facility/fclMisc/decls.h"

s32 fclChkMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    if (gMsgInvFlag != 0 && gMsgFlag == 2) {
        return 0;
    }
    return (s8)(u8)gMsgFlag;
}
