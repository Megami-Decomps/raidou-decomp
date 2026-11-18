#include "common.h"
#include "dds3ObjectBase.h"

#include "basic.h"
#include "dds3obj.h"
#include "field.h"
#include "include_asm.h"
#include "kernel_draw.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "ps2asm.h"
#include "fpu.h"
#include "kernel.h"
#include "draw3d.h"
#include "scr.h"
#include "fldFileResolver_00132080.h"
#include "effPCPMisc_00224D10.h"
#include "fldFileResolver.h"
#include "datCalc.h"
#include "newdata.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "mdlManager.h"
#include "fldDbg.h"
#include "sdfQuat.h"
#include "sdfMath.h"
#include "sdk_libm.h"
#include "sdfMemory.h"
#include "dds3KernelCore.h"
#include "sdf3D.h"
#include "EventBit.h"
#include "sdfTex.h"
#include "effManager.h"
#include "dds3KernelDraw.h"
#include "dds3KernelDraw_0010A750.h"
#include "dds3KernelMain.h"
#include "sdfAspect.h"

INCLUDE_ASM(const s32, "code_00156EA8", fldLOADTMY);

void fldCallMemFreeDelayedDirect4(void) {
    if (evtStageTestState2.handle != NULL) {
        sdfMemFreeDelayedDirect(evtStageTestState2.handle);
        evtStageTestState2.handle = NULL;
    }
    fldData23 = 0;
}

/* NON_MATCHING: 485 of 505 words match.  */
#ifdef NON_MATCHING
void fldCallLinkAlpha(u32 color, f32 x, f32 y) {
    s32 first = 1;
    f32 size;
    f32 cx;
    f32 cy;
    s32 row;
    s32 col;
    s32 idx;

    if (evtStageTestState2.handle == NULL) {
        return;
    }
    size = Zoom * 126.0f;
    if (fldGlobalBase.camId == 0) {
        switch (dds3LinkBoxZSub()) {
        case 0:
            size += size;
            break;
        case 1:
            size += size;
            x -= 512.0f;
            break;
        case 2:
            size += size;
            y -= 448.0f;
            break;
        case 3:
            size += size;
            x -= 512.0f;
            y -= 448.0f;
            break;
        }
    } else {
        switch (dds3LinkBoxZSub()) {
        case 0:
            size += size;
            x += x;
            y += y;
            break;
        case 1:
            x += x;
            size += size;
            y += y;
            x -= 512.0f;
            break;
        case 2:
            y += y;
            size += size;
            x += x;
            y -= 448.0f;
            break;
        case 3:
            x += x;
            y += y;
            size += size;
            x -= 512.0f;
            y -= 448.0f;
            break;
        }
    }
    if (evtStageTestState2.head->entrySize < 0 && fldData22 != 2) {
        eglLinkAlpha(3);
        if (fldData22 == 3) {
            eglLinkAlpha(0);
        }
        evtSubmitDefaultDepthGradientRect(SDF_ASPECT_LEFT, 0, SDF_ASPECT_RIGHT - SDF_ASPECT_LEFT, 448, 0x80000000, 0x80000000, 0x80000000, 0x80000000);
        idx = 0;
        cy = y;
        for (row = 0; row < evtStageTestState2.sub->h; row++) {
            cx = x;
            for (col = 0; col < evtStageTestState2.sub->w; col++, idx++) {
                if (!(cx + size < SDF_ASPECT_LEFT || cx > SDF_ASPECT_RIGHT || cy + size < 0.0f || cy > 448.0f) && evtStageTestState2.cell[idx] != NULL) {
                    if (first) {
                        first = 0;
                        fldCallGrAllocPacketBuff4(evtStageTestState2.cell[idx]);
                    } else {
                        fldCallGrAllocPacketBuff5(evtStageTestState2.cell[idx]);
                    }
                    eglCallDLinkTexPacket(cx, cy, size, size, 1, 1, 126, 126, 0x80808080, 0x80808080, 0x80808080, 0x80808080, fldTitleMiniWork2);
                }
                cx += size;
            }
            cy += size;
        }
        eglLinkAlpha(9);
        if (fldData22 == 3) {
            evtSubmitDefaultDepthGradientRect(SDF_ASPECT_LEFT, 0, SDF_ASPECT_RIGHT - SDF_ASPECT_LEFT, 448, 0x80808080, 0x80808080, 0x80808080, 0x80808080);
        }
    }
    if (fldData22 == 3) {
        return;
    }
    if (fldData22 == 2) {
        eglLinkAlpha(0);
        eglLinkTest(1, 0, 0, 1, 0, 0, 1, 1);
        evtSubmitDefaultDepthGradientRect(SDF_ASPECT_LEFT, 0, SDF_ASPECT_RIGHT - SDF_ASPECT_LEFT, 448, 0x80000000, 0x80000000, 0x80000000, 0x80000000);
        eglLinkAlpha(1);
        eglLinkTest(1, 0, 0, 1, 0, 0, 1, 1);
        first = 1;
        idx = 0;
        cy = y;
        for (row = 0; row < evtStageTestState2.head->h; row++) {
            cx = x;
            for (col = 0; col < evtStageTestState2.head->w; col++, idx++) {
                if (!(cx + size < SDF_ASPECT_LEFT || cx > SDF_ASPECT_RIGHT || cy + size < 0.0f || cy > 448.0f)) {
                    if (first) {
                        first = 0;
                        fldCallGrAllocPacketBuff2(evtStageTestState2.entry[idx]);
                    } else {
                        fldCallGrAllocPacketBuff3(evtStageTestState2.entry[idx]);
                    }
                    eglCallDLinkTexPacket(cx, cy, size, size, 1, 1, 126, 126, 0x80000000, 0x80000000, 0x80000000, 0x80000000, fldTitleMiniTex2);
                }
                cx += size;
            }
            cy += size;
        }
        eglLinkTest(1, 0, 0, 3, 0, 0, 1, 1);
        eglLinkAlpha(9);
        evtSubmitDefaultDepthGradientRect(SDF_ASPECT_LEFT, 0, SDF_ASPECT_RIGHT - SDF_ASPECT_LEFT, 448, 0x80808080, 0x80808080, 0x80808080, 0x80808080);
        return;
    }
    {
        if (fldData22 == 1) {
            eglLinkAlpha(4);
        }
        first = 1;
        idx = 0;
        cy = y;
        for (row = 0; row < evtStageTestState2.head->h; row++) {
            cx = x;
            for (col = 0; col < evtStageTestState2.head->w; col++, idx++) {
                if (!(cx + size < SDF_ASPECT_LEFT || cx > SDF_ASPECT_RIGHT || cy + size < 0.0f || cy > 448.0f)) {
                    if (first) {
                        first = 0;
                        fldCallGrAllocPacketBuff2(evtStageTestState2.entry[idx]);
                    } else {
                        fldCallGrAllocPacketBuff3(evtStageTestState2.entry[idx]);
                    }
                    eglCallDLinkTexPacket(cx, cy, size, size, 1, 1, 126, 126, color, color, color, color, fldTitleMiniTex2);
                }
                cx += size;
            }
            cy += size;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00156EA8", fldCallLinkAlpha);
#endif
