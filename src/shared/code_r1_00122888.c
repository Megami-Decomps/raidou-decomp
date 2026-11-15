#include "common.h"
#include "scrTraceCode.h"

extern s32 datGetStockDevilNums(void);

s32 scrCmdHasEnoughOwnedMantras(void) {
    scrSetIntReturnValue(datGetStockDevilNums());
    return 1;
}
