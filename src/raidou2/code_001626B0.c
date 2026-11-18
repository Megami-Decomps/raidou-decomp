#include "common.h"
#include "mapLbWork.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdfGsMemIO.h"
#include "kernel_draw.h"
#include "dds3KernelDraw.h"
#include "sdfMemory.h"
#include "libsdf.h"
#include "sdf_resource.h"
#include "ps2asm.h"

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

/* NON_MATCHING: 83 of 88 words match.  */
#ifdef NON_MATCHING
void fldCallGrAllocPacketBuff4(u8 *image) {
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s32 widthLog2;
    s32 heightLog2;
    sdfTexHandle_t *tex;
    u32 primary;
    u32 secondary;
    u8 clutFormat;
    u8 pixelFormat;

    if (fldTitleMiniWork2 != NULL && image != NULL) {
        tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(tag);
        sdfLinkLoadImage(tag, fldTitleMiniWork2->clutGsHandle, 0, 0, 16, 16, (saddr_t)(image + 0x40), 0);
        sdfLinkLoadImage(tag, fldTitleMiniWork2->pixelGsHandle, 0, 0, 128, 128, (saddr_t)(image + 0x440), 0);
        ot = &sdfGrOtTable[egl_OT_Offset];
        ot->regFunc(ot, tag);
        tex = fldTitleMiniWork2;
        widthLog2 = tex->width - 1;
        primary = tex->pixelGsHandle->addr >> 6;
        heightLog2 = tex->height - 1;
        clutFormat = tex->CLUTFormat;
        secondary = tex->clutGsHandle->addr;
        pixelFormat = tex->pixelFormat;
        EE_MMI_BIT_LENGTH(heightLog2);
        EE_MMI_BIT_LENGTH(widthLog2);
        secondary >>= 6;
        tex->packet->tex0.data =
            ((u64)2 << 61) |
            ((u64)clutFormat << 51) |
            ((u64)secondary << 37) |
            ((u64)1 << 34) |
            ((u64)heightLog2 << 30) |
            (widthLog2 << 26) |
            ((u64)pixelFormat << 20) |
            (2 << 14) |
            (u64)primary;
    }
}
#else
INCLUDE_ASM(const s32, "code_001626B0", fldCallGrAllocPacketBuff4);
#endif

void fldCallGrAllocPacketBuff5(u8 *image) {
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;

    if (fldTitleMiniWork2 != NULL && image != NULL) {
        tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(tag);
        sdfLinkLoadImage(tag, fldTitleMiniWork2->pixelGsHandle, 0, 0, 128, 128, (saddr_t)(image + 0x80), 0);
        ot = &sdfGrOtTable[egl_OT_Offset];
        ot->regFunc(ot, tag);
    }
}

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
