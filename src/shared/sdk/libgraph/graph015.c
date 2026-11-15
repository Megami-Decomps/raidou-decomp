#include "common.h"
#include "ps2asm.h"
#include "sdk_libgraph.h"
#include "sdk_libkernl.h"

/* Reference implementation; see docs/matching/sdk/libgraph.md. */

s32 sceGsSetDefLoadImage(sceGsLoadImage *p, s16 dbp, s16 dbw, s16 dpsm, s16 dsax, s16 dsay,
                         s16 rrw, s16 rrh)
{
    s32 qwc = 0;

    switch (dpsm) {
    case 0x00:
    case 0x30:
        qwc = (rrw * rrh) >> 2;
        break;
    case 0x01:
    case 0x31:
        qwc = (rrw * rrh * 3) >> 4;
        break;
    case 0x02:
    case 0x0A:
    case 0x32:
    case 0x3A:
        qwc = (rrw * rrh) >> 3;
        break;
    case 0x13:
    case 0x1B:
        qwc = (rrw * rrh) >> 4;
        break;
    case 0x14:
    case 0x24:
    case 0x2C:
        qwc = (rrw * rrh) >> 5;
        break;
    }
    if (qwc > 0x7FFF) {
        scePrintf("sceGsSetDefLoadImage: too big size\r\n");
        return 0;
    }
    p->tag.qword = 0;
    p->tag.f.NLOOP = 4;
    p->tag.f.NREG = 1;
    p->tag.f.REGS0 = 0xE;
    p->bitbltbuf = ((u64)dbp << 32) | ((u64)dbw << 48) | ((u64)dpsm << 56);
    p->reg_bitbltbuf = 0x50;
    p->trxpos = ((u64)dsax << 32) | ((u64)dsay << 48);
    p->reg_trxpos = 0x51;
    p->trxreg = (u64)rrw | ((u64)rrh << 32);
    p->reg_trxreg = 0x52;
    p->trxdir = 0;
    p->reg_trxdir = 0x53;
    p->imagetag.qword = 0;
    p->imagetag.f.NLOOP = qwc;
    p->imagetag.f.EOP = 1;
    p->imagetag.f.FLG = 2;
    EE_SYNC();
    return 6;
}
