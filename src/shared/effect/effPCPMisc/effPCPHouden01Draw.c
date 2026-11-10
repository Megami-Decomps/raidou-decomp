#include "shared/effect/effPCPMisc/decls.h"

void effPCPHouden01Draw(effPCPShockWave01_tag *hData) {
    effPCPThunderSpherePolyParam_t *parameterHead;
    f32 phase;
    f32 value;
    u32 fadedColor;
    u32 frame;

    frame = hData->frame;
    if (frame == SDF_FRAMES(0x41)) {
        return;
    }
    if (frame < SDF_FRAMES(0x1E)) {
        parameterHead = effPCPThunderSpherePolyGetParam(hData->pThunderPoly);
        phase = (f32)frame / SDF_FRAMES_F(30.0f);
        value = phase * 200.0f + 150.0f;
        parameterHead->width = value;
        parameterHead->height = value;
    }
    if (frame >= SDF_FRAMES(0x39)) {
        phase = ((f32)frame - SDF_FRAMES_F(57.0f)) * SDF_FRAME_STEP(0.125f);
        fadedColor = effColorBlend(hData->abgr, 0, phase);
        effPCPThunderSpherePolySetColor(hData->pThunderPoly, fadedColor);
    } else {
        effPCPThunderSpherePolySetColor(hData->pThunderPoly, hData->abgr);
    }
    effPCPThunderSpherePolySetPosition((void *)hData->pThunderPoly, hData);
    effPCPThunderSpherePolyDraw(hData->pThunderPoly);
    hData->frame++;
}
