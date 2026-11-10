#include "shared/sdf/sdfDev9/decls.h"

s32 sdfBcdToDecimal(s32 aBcd)
{
    s32 result = 0;
    s32 scale = 1;

    while (aBcd > 0) {
        result += (aBcd & 0xF) * scale;
        aBcd >>= 4;
        scale *= 10;
    }
    return result;
}
