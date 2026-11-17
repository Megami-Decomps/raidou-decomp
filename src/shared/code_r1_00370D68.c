#include "common.h"
#include "sdfSound.h"

INCLUDE_ASM(const s32, "code_r1_00370D68", sdfCallSndTransToIOP);

INCLUDE_ASM(const s32, "code_r1_00370D68", sdfCallSndSendSyncCmd);

INCLUDE_ASM(const s32, "code_r1_00370D68", sdfCallSndSendSyncCmd2);

void sdfCallSndSendCmd(s32 arg) {
    sdfSndSendCmd(0x230, 0, &arg, 0x10);
}

/* body: matched in raidou @ 88f0d54: src/shared/sdf/sdfSound.c:524, near, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
s32 sdfCallSndSendCmd2(s32 aId)
{
    s32 arg[4];

    arg[0] = aId;
    return sdfSndSendCmd(0x240, 0, arg, sizeof(arg));
}

s32 sdfBtlReLoadMapSBtlSceneSub(void) {
    return sdfSndSendCmd(0x250, 0, NULL, 0);
}
