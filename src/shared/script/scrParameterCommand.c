#include "common.h"
#include "datCalc.h"
#include "scrTraceCode.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/script/scrParameterCommand.c:60, match, same function 1.00 (words 16/16, calls 1.00, shape 1.00) */
s32 prm_ITEM_PLUS(void)
{
    s32 itemId = scrGetIntPara(0);
    s32 amount = scrGetIntPara(1);

    datAddItem(itemId, amount);
    return 1;
}

INCLUDE_ASM(const s32, "script/scrParameterCommand", scrCallGetIntPara5);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_UDV_GETID);

#endif
INCLUDE_ASM(const s32, "script/scrParameterCommand", scrCallGetIntPara6);

/* body: matched in smt3 @ 9401f79e: src/smt3/script/scrParameterCommand.c:99, match, same function 0.92 (words 18/18, calls 0.75, shape 1.00) */
s32 prm_MAKA_CHECK(void)
{
    if (datCheckMag(scrGetIntPara(0)) != 0) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_NAKAMA_DEAD);
