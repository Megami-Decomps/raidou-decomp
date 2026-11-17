#include "common.h"
#include "fclDrawBack.h"

void fclLeaf6(fclMsgWork_t *wk) {
    fclCallMemFreeDirect(wk);
}

INCLUDE_ASM(const s32, "fclDrawBack", fclCallGetProcessData);

INCLUDE_ASM(const s32, "fclDrawBack", fclProcessStart);

INCLUDE_ASM(const s32, "fclDrawBack", fclProcessEnd);

INCLUDE_ASM(const s32, "fclDrawBack", dds3DestroyCallbackNodeAfterLastNotification);

INCLUDE_ASM(const s32, "fclDrawBack", fclCallSetOt);

INCLUDE_ASM(const s32, "fclDrawBack", _BackSprite);

INCLUDE_ASM(const s32, "fclDrawBack", fclBackSprite);

INCLUDE_ASM(const s32, "fclDrawBack", fclCommandHelp);

INCLUDE_ASM(const s32, "fclDrawBack", fldAllTblLoad);

INCLUDE_ASM(const s32, "fclDrawBack", fclCallNewGameInit);
