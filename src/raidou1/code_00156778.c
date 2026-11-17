#include "common.h"
#include "mapLbWork.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdfGsMemIO.h"
#include "libsdf.h"
#include "sdf_resource.h"
#include "kernel_draw.h"
#include "dds3KernelDraw.h"
#include "ps2asm.h"

/* NON_MATCHING: 86 of 88 words match.  */
#ifdef NON_MATCHING
void fldCallGrAllocPacketBuff2(u8 *image) {
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s32 widthLog2;
    s32 heightLog2;
    sdfTexHandle_t *tex;
    u32 primary;
    u32 secondary;
    u8 clutFormat;
    u8 pixelFormat;

    if (fldTitleMiniTex2 != NULL && image != NULL) {
        tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(tag);
        sdfLinkLoadImage(tag, fldTitleMiniTex2->clutGsHandle, 0, 0, 16, 16, (saddr_t)(image + 0x40), 0);
        sdfLinkLoadImage(tag, fldTitleMiniTex2->pixelGsHandle, 0, 0, 128, 128, (saddr_t)(image + 0x440), 0);
        ot = &sdfGrOtTable[egl_OT_Offset];
        ot->regFunc(ot, tag);
        tex = fldTitleMiniTex2;
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
INCLUDE_ASM(const s32, "code_00156778", fldCallGrAllocPacketBuff2);
#endif

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/001568D8.c:1, found */
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

INCLUDE_ASM(const s32, "code_00156778", fldCallGrAllocPacketBuff4);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/00156AD0.c:1, match */
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
