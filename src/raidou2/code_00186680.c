#include "common.h"
#include "sdfTex.h"
#include "sdfSound.h"
#include "effBlur_Filter.h"
#include "fldEnc.h"
#include "dds3KernelDraw.h"
#include "fldDraw.h"

INCLUDE_ASM(const s32, "code_00186680", encLoadTexture);

void fldFreeRunMapData(void)
{
    if (fldData42 != NULL) {
        sdfTexFreeDelayed(fldData42);
        fldData42 = NULL;
        fldCallSetOt5Data = 0.0f;
        fldCallSetOt5Data2 = 0.0f;
    }
}

void fldCallSetOt5(void)
{
    if (fldData42 == NULL) {
        return;
    }
    if (fldCallSetOt5Data + 256.0f + 128.0f + 128.0f + 256.0f + 200.0f < 0.0f) {
        return;
    }
    eglSetOt(101);
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 128, 1, 0, 0, 1, 1);
    eglCallDLinkTexPacket2(fldCallSetOt5Data + 0.0f, 0.0f, 968.0f, 448.0f, 0.0f, 0.0f, 256.0f, 256.0f,
                           0x80808080, 0x80808080, 0x80808080, 0x80808080, fldData42);
    fldCallSetOt5Data -= fldCallSetOt5Data2 + 4.0f;
    fldCallSetOt5Data2 += 2.5f;
}

INCLUDE_ASM(const s32, "code_00186680", encDrawBokasi0);

INCLUDE_ASM(const s32, "code_00186680", encCallPrimAllocDrawTag);

INCLUDE_ASM(const s32, "code_00186680", encCallPrimAllocDrawTag2);

INCLUDE_ASM(const s32, "code_00186680", encLoad);

void encMode(s32 mode)
{
    gencMode = mode;
    if (mode == 1) {
        p2sdPlaySE(14, 127, 63);
    }
}

s32 encDrawProc(dds3ProcessID_t *aProcId)
{
    if (gencOn == 0) {
        return 0;
    }
    if (gencMode == 0) {
        rsetOtOfs(90);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        rsetAlpha(0);
        encCallPrimAllocDrawTag();
        aDrawBoxF(0, 0xDF, 0x200, 2, 0, 0, 0, 0x80);
        rsetAlpha(0);
        encDrawState = encDrawState + 1;
    }
    if (gencMode == 1) {
        if (gencStageInnAlp > 0) {
            rsetOtOfs(101);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            rsetAlpha(0);
            encCallPrimAllocDrawTag2(0x80);
            aDrawBoxF(0, 0xDF, 0x200, 2, 0, 0, 0, 0x80);
            gencStageInnAlp = gencStageInnAlp - 1;
        } else {
            encStop();
            return 0;
        }
        fldCallSetOt5();
    }
    return 0;
}

void encStop(void)
{
    gencOn = 0;
    if (hPartialMultiBlur != NULL) {
        effStopSub(hPartialMultiBlur);
        hPartialMultiBlur = NULL;
    }
    fldFreeRunMapData();
}
