#include "shared/effect/effPCPMisc/decls.h"

void effPCPMegidora01Draw(effPCPShockWave01_tag *hData) {
    effPCPThunderSpherePolyParam_t *parameterHead;
    f32 phase;
    f32 value;
    u32 fadedColor;
    u32 frame;

    frame = hData->frame;
    if (frame < SDF_FRAMES(0x1F)) {
        parameterHead = effPCPThunderSpherePolyGetParam(hData->pThunderPoly);
        frame = hData->frame;
        phase = (f32)frame / SDF_FRAMES_F(30.0f);
        value = phase * 150.0f + 100.0f;
        parameterHead->width = value;
        parameterHead->height = value;
    } else if (frame - SDF_FRAMES(0x2D) < SDF_FRAMES(0x10)) {
        parameterHead = effPCPThunderSpherePolyGetParam(hData->pThunderPoly);
        frame = hData->frame;
        phase = (f32)(frame - SDF_FRAMES(0x2D)) / SDF_FRAMES_F(15.0f);
        value = phase * 150.0f + 250.0f;
        parameterHead->width = value;
        parameterHead->height = value;
    }
    if (frame >= SDF_FRAMES(0x2D)) {
        phase = (f32)(frame - SDF_FRAMES(0x2D)) / SDF_FRAMES_F(35.0f);
        fadedColor = effColorBlend(hData->abgr, 0, phase);
    } else {
        fadedColor = hData->abgr;
    }
    effPCPThunderSpherePolySetColor(hData->pThunderPoly, fadedColor);
    effPCPThunderSpherePolySetPosition((void *)hData->pThunderPoly, hData);
    effPCPThunderSpherePolyDraw(hData->pThunderPoly);
    hData->frame++;
}
