#include "shared/interface/itfMesManager/decls.h"

s32 GetNumSelectItem(s32 allitem, u32 mask)
{
    s32 n = 0;

    while (allitem > 0) {
        u32 bit = mask & 1;

        mask >>= 1;
        allitem--;
        if (bit == 0) {
            n++;
        }
    }
    return n;
}
