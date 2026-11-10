#include "shared/event/evtCommand/decls.h"

u32 evtCommand_CHK_BE(void)
{
    s32 no;
    s32 ret;

    no = scrGetIntPara(0);
    if (scrGetScriptHandle() != NULL && evtSearchEventPack(no) != 0
            && evtGetEventPackStat(no) == 2) {
        datDebugPrintf(evtMsgBeOk, no);
        ret = 1;
    } else {
        ret = 0;
    }
    scrSetIntReturnValue(ret);
    return 1;
}
