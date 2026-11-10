#include "shared/effect/effPCPMisc/decls.h"

void effPCPAgiFireball2Draw(effPCPAgiFireball2_tag *hData) {
    s32 frame;
    u32 i;

    i = 0;
    frame = hData->frame;
    do {
        if (SDF_FRAMES(hData->params.startFrame[i]) <= frame) {
            effPCPAgiFireballDraw(hData->blockSets[i]);
            frame = hData->frame;
        }
        i = i + 1;
    } while (i < 3);
    hData->frame = frame + 1;
}
