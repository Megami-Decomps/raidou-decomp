#include "common.h"
#include "mapLbWork.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdfGsMemIO.h"
#include "kernel_draw.h"
#include "dds3KernelDraw.h"
#include "sdfMemory.h"

INCLUDE_ASM(const s32, "code_001626B0", fldCallGrAllocPacketBuff2);

void fldCallGrAllocPacketBuff3(u8 *image) {
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;

    if (fldTitleMiniTex2 != NULL && image != NULL) {
        tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(tag);
        sdfLinkLoadImage(tag, fldTitleMiniTex2->pixelGsHandle, 0, 0, 128, 128, (saddr_t)(image + 0x440), 0);
        ot = &sdfGrOtTable[egl_OT_Offset];
        ot->regFunc(ot, tag);
    }
}

INCLUDE_ASM(const s32, "code_001626B0", fldCallGrAllocPacketBuff4);

INCLUDE_ASM(const s32, "code_001626B0", fldCallGrAllocPacketBuff5);

INCLUDE_ASM(const s32, "code_001626B0", fldJmsQuestMapLoad_fld);

INCLUDE_ASM(const s32, "code_001626B0", fldLOADTMY);

void fldCallMemFreeDelayedDirect4(void) {
    if (evtStageTestState2.handle != NULL) {
        sdfMemFreeDelayedDirect(evtStageTestState2.handle);
        evtStageTestState2.handle = NULL;
    }
    fldData23 = 0;
}

INCLUDE_ASM(const s32, "code_001626B0", fldCallLinkAlpha);
