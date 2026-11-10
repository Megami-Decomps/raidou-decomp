#include "shared/sdf/sdfDevCons/decls.h"

void *sdfDevPrintfEx(u32 *aCB, const char *aStr, ...)
{
    __builtin_va_list args;

    __builtin_stdarg_start(args, aStr);
    return sdfDevVprintf(aCB, aStr, args);
}
