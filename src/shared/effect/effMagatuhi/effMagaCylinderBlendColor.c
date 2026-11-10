#include "shared/effect/effMagatuhi/decls.h"

/* Accumulate 1/historyCount per entry; the nominal last fraction is below 1. */
void effMagaCylinderBlendColor(dds3MagatuhiBill_tag *work, u32 colorA, u32 colorB) {
    f32 blendFraction = 0.0f;
    u32 historyCount = work->historyCount;
    u32 *paletteEntry = work->colorTable;
    f32 blendStep = 1.0f / historyCount;
    u32 paletteEntryIndex;

    for (paletteEntryIndex = 0; paletteEntryIndex < historyCount; paletteEntryIndex++) {
        *paletteEntry++ = effColorBlend(colorA, colorB, blendFraction);
        blendFraction += blendStep;
    }
}
