#include "common.h"
#include "sdfTex.h"
#include "sdfSound.h"
#include "effBlur_Filter.h"
#include "fldEnc.h"

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

INCLUDE_ASM(const s32, "code_00186680", fldCallSetOt5);

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

INCLUDE_ASM(const s32, "code_00186680", encDrawProc);

void encStop(void)
{
    gencOn = 0;
    if (hPartialMultiBlur != NULL) {
        effStopSub(hPartialMultiBlur);
        hPartialMultiBlur = NULL;
    }
    fldFreeRunMapData();
}
