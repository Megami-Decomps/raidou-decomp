#include "common.h"

extern s32 datGetStockDevilNums(void);
extern void scrSetIntReturnValue(s32 value);

s32 scrCmdHasEnoughOwnedMantras(void) {
    scrSetIntReturnValue(datGetStockDevilNums());
    return 1;
}
