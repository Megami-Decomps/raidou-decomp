#include "common.h"
#include "fclDrawBack.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"

void fclLeaf6(fclMsgWork_t *wk) {
    fclCallMemFreeDirect(wk);
}

s32 fclCallGetProcessData(dds3ProcessID_t *PID) {
    fclMsgWork_t *wk = dds3GetProcessData(PID);

    fclLeaf6(wk);
    sdfMemFreeDirect(wk->hMem);
    FAKEMATCH_ASM_READ(wk);
#ifdef FIX_BUGS
    return 0;
#endif
}

INCLUDE_ASM(const s32, "fclDrawBack", fclProcessStart);

INCLUDE_ASM(const s32, "fclDrawBack", fclProcessEnd);

INCLUDE_ASM(const s32, "fclDrawBack", dds3DestroyCallbackNodeAfterLastNotification);

INCLUDE_ASM(const s32, "fclDrawBack", fclCallSetOt);

INCLUDE_ASM(const s32, "fclDrawBack", _BackSprite);

INCLUDE_ASM(const s32, "fclDrawBack", fclBackSprite);

INCLUDE_ASM(const s32, "fclDrawBack", fclCommandHelp);

INCLUDE_ASM(const s32, "fclDrawBack", fldAllTblLoad);

INCLUDE_ASM(const s32, "fclDrawBack", fclCallNewGameInit);
