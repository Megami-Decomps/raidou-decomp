#include "common.h"
#include "fldEnc.h"
#include "dds3KernelCore.h"
#include "dds3WorldBasic.h"
#include "effBlur_Filter.h"
#include "fldProcess.h"

void encStart(void) {
    if (encLoad() != 0) {
        return;
    }
    gencStageInnAlp = 0x80;
    encBlurParam.Blur.abgr = 0x09808080;
    encBlurParam.Blur.blend = 0x44;
    encBlurParam.Blur.rotate = 0.01f;
    encBlurParam.Blur.scale = 1.02f;
    encBlurParam.area = 0x12C;
    gencMode = 0;
    encDrawState = 0;
    encBlurParam.Blur.center[0] = 0;
    encBlurParam.Blur.center[1] = 0;
    encBlurParam.Blur.xy[0][0] = 0;
    encBlurParam.Blur.xy[0][1] = 0;
    encBlurParam.Blur.xy[1][0] = 0;
    encBlurParam.Blur.xy[1][1] = 0;
    hPartialMultiBlur = dds3EffectPartialMultiBlurCreate(&encBlurParam);
    gencOn = 1;
    if (dds3GetActiveScene() != NULL) {
        dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
    }
    encLoadTexture();
}

void encInitProcess(void) {
    dds3StartProcess(fldEncDrawName, 0x2B0D, 0, 1, encDrawProc, NULL, NULL);
}

INCLUDE_SDATA(const s32, "field/fldEnc", fldEncDrawName);

