#include "common.h"

extern s32 func_00122848(void);
extern void scrSetIntReturnValue(s32 value);

s32 func_00126B08(void) {
    scrSetIntReturnValue(func_00122848());
    return 1;
}
