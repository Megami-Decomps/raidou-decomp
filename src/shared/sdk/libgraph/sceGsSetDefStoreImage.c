#include "shared/sdk/libgraph/decls.h"

s32 sceGsSetDefStoreImage(sceGsStoreImage *p, s16 sbp, s16 sbw, s16 spsm, s16 ssax, s16 ssay,
                          s16 rrw, s16 rrh)
{
    p->qwc = 0;
    p->addr = 0x06008000;
    p->vifcode0 = 0x13000000;
    p->vifcode1 = 0x50000006;
    p->tag.qword = 0;
    p->tag.f.NLOOP = 5;
    p->tag.f.EOP = 1;
    p->tag.f.NREG = 1;
    p->tag.f.REGS0 = 0xE;
    p->bitbltbuf = (u64)sbp | ((u64)sbw << 16) | ((u64)spsm << 24);
    p->reg_bitbltbuf = 0x50;
    p->trxpos = (u64)ssax | ((u64)ssay << 16);
    p->reg_trxpos = 0x51;
    p->trxreg = (u64)rrw | ((u64)rrh << 32);
    p->reg_trxreg = 0x52;
    p->finish = 0;
    p->reg_finish = 0x61;
    p->trxdir = 1;
    p->reg_trxdir = 0x53;
    EE_SYNC();
    return 7;
}
