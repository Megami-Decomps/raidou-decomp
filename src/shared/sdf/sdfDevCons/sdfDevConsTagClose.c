#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsTagClose(sdfDevConsDrawCB_t *aCB)
{
    u8 *cur = aCB->cur;
    volatile u64 *tag = (volatile u64 *)aCB->tag;
    s32 regCount = ((cur - aCB->tag) >> 3) - 2;

    if (regCount != 0) {
        if (regCount & 1) {
            cur += 8;
        }
        regCount /= 5;
        aCB->tag = cur;
        aCB->cur = cur + 0x10;
        tag[0] = (s64)regCount | 0x5400000000008000ULL;
        tag[1] = 0x53531;
    }
}
