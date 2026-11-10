#include "shared/facility/fclMisc/decls.h"

s8 fclInitMessage(void *pData)
{
    s32 ret;

    if (gMsgID < 0) {
        gMsgID = itfMesMngInitialize(pData);
        itfMesMngChangeWindowType(gMsgID, 2, 0);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}
