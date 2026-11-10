#include "shared/effect/effPCPMisc/decls.h"

void effPCPShockWave01Draw(effPCPShockWave01_tag *hData) {
    effPCPThunderSpherePolyParam_t *parameterHead;
    f32 phase;
    u32 fadedColor;
    u32 frame;

    frame = hData->frame;
    if (frame < SDF_FRAMES(0x17)) {
        parameterHead = effPCPThunderSpherePolyGetParam(hData->pThunderPoly);
        frame = hData->frame;
        phase = (f32)frame / SDF_FRAMES_F(22.0f);
        parameterHead->width = phase * 600.0f + 200.0f;
        parameterHead->height = phase * 300.0f + 100.0f;
    }
    if (frame >= SDF_FRAMES(0x2D)) {
        phase = (f32)(frame - SDF_FRAMES(0x2D)) / SDF_FRAMES_F(30.0f);
        fadedColor = effColorBlend(hData->abgr, 0, phase);
    } else {
        fadedColor = hData->abgr;
    }
    effPCPThunderSpherePolySetColor(hData->pThunderPoly, fadedColor);
    effPCPThunderSpherePolySetPosition((void *)hData->pThunderPoly, hData);
    effPCPThunderSpherePolyDraw(hData->pThunderPoly);
    hData->frame++;
}
