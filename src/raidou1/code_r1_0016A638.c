#include "common.h"
#include "dds3KernelDraw.h"
extern s32 mapData134;
extern s32 fldCallSetOt2Data;

extern void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 w, s32 h, u32 c0, u32 c1, u32 c2, u32 c3);

void mapProcShutDownSub2(s32 mode) {
    if (mode == 0) {
        mapData134 = 0;
        fldCallSetOt2Data = 0x80;
    } else if (mode == -1) {
        mapData134 = mode;
    } else {
        mapData134 = 1;
    }
}

void fldCallSetOt2(void) {
    s32 mode = mapData134;

    if (mode == -1) {
        if (fldCallSetOt2Data + 6 < 0x61) {
            fldCallSetOt2Data += 6;
        } else {
            fldCallSetOt2Data = 0x60;
        }
    } else if (mode == 1) {
        if (fldCallSetOt2Data - 6 < 0) {
            fldCallSetOt2Data = 0;
            mode = 0;
            mapData134 = 0;
        } else {
            fldCallSetOt2Data -= 6;
        }
    }
    if (mode != 0) {
        eglSetOt(0x67);
        eglLinkAlpha(0);
        eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        evtSubmitDefaultDepthGradientRect(0, 0, 0x200, 0x1C0, fldCallSetOt2Data << 24, fldCallSetOt2Data << 24, fldCallSetOt2Data << 24, fldCallSetOt2Data << 24);
    }
}
INCLUDE_SDATA(const s32, "code_r1_0016A638", mapData134);

INCLUDE_SDATA(const s32, "code_r1_0016A638", fldCallSetOt2Data);

INCLUDE_SDATA(const s32, "code_r1_0016A638", fldFunc18Data);

