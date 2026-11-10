#include "shared/sdf/sdfDevCons/decls.h"

s32 sdfDevConsPrintf(sdfDevConsCB_tag *aHndl, const char *aFmt, ...)
{
    char buf[0x200];
    __builtin_va_list args;
    s32 count = 0;
    s32 ch;

    __builtin_stdarg_start(args, aFmt);
    vsprintf(buf, aFmt, args);
    ch = buf[0];
    if (ch != 0) {
        do {
            count++;
            sdfDevConsPutInside(aHndl, ch, aHndl->attr);
            ch = buf[count];
        } while (ch != 0);
    }
    return count;
}
