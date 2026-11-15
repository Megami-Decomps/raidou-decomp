#include "common.h"
#include "sdfSound.h"

INCLUDE_ASM(const s32, "code_r1_00370D68", sdfCallSndTransToIOP);

INCLUDE_ASM(const s32, "code_r1_00370D68", sdfCallSndSendSyncCmd);

INCLUDE_ASM(const s32, "code_r1_00370D68", sdfCallSndSendSyncCmd2);

void sdfCallSndSendCmd(s32 arg) {
    sdfSndSendCmd(0x230, 0, &arg, 0x10);
}

INCLUDE_ASM(const s32, "code_r1_00370D68", sdfCallSndSendCmd2);

s32 sdfBtlReLoadMapSBtlSceneSub(void) {
    return sdfSndSendCmd(0x250, 0, NULL, 0);
}
