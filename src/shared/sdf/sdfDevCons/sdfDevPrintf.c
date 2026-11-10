#include "shared/sdf/sdfDevCons/decls.h"

void *sdfDevPrintf(u32 aX, u32 aY, u32 aZ, s32 aAttr, const char *aStr, ...)
{
    u32 cb[4];
    __builtin_va_list args;

    sdfDevSetPrintCB(cb, aX, aY, aZ, aAttr);
    __builtin_stdarg_start(args, aStr);
    return sdfDevVprintf(cb, aStr, args);
}
