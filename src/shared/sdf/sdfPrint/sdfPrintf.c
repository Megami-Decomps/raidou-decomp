#include "shared/sdf/sdfPrint/decls.h"

s32 sdfPrintf(const char *str, ...) {
    char buffer[0x100];
    __builtin_va_list args;
    s32 length;
    __builtin_stdarg_start(args, str);
    length = vsprintf(buffer, str, args);
    sdfPuts(buffer);
    return length;
}
