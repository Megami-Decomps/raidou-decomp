#include "common.h"
#include "code_r1_002CAFD8.h"

void uiPrepareMenuBackgroundSurface(s32 unused, s32 ot) {
    cmpCallGrAllocPacketBuff2(ot);
    fclDrawPanelSprite(0x50000, ot);
    fclDrawPanelSprite2(0x54, ot);
    FAKEMATCH_ASM_READ(ot);
}
