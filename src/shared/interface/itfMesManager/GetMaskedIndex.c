#include "shared/interface/itfMesManager/decls.h"

s32 GetMaskedIndex(s32 index, u32 mask)
{
    s32 i = 0;

    while (i < 0x20) {
        if ((mask & 1) == 0) {
            if (--index < 0) {
                break;
            }
        }
        i++;
        mask >>= 1;
    }
    return i;
}
