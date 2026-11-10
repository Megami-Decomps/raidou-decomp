#include "shared/interface/itfMesManager/decls.h"

void itfMesMngSetSelect(s32 id, s32 selno)
{
    select_tag *blk = &pinst[id].mes->select;
    u32 mask = blk->mask;
    s32 bit = mask & 1;
    s32 n = bit == 0;
    s32 i = 0;

    if (selno > 0) {
        do {
            i++;
            mask >>= 1;
            bit = mask & 1;
            if (bit == 0) {
                n++;
            }
        } while (i < selno);
    }
    n = bit == 0 ? n - 1 : 0;
    blk->select = n;
    blk->select_imm = n;
}
