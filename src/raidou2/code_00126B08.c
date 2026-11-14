#include "common.h"

extern s32 func_r1_0011F720(void);
extern void scrSetIntReturnValue(s32 value);

s32 scrCmdHasEnoughOwnedMantras(void) {
    scrSetIntReturnValue(func_r1_0011F720());
    return 1;
}
