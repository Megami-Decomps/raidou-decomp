#include "shared/effect/parManager/decls.h"

/* Draw live particles after applying the requested number of emitter updates. */
void dds3ParticleDraw(dds3Particle_Basic_t *hData) {
    effectBufferRecord_t *record;
    billHandle_t *billboard;
    u32 step;
    s32 particleCount;
    s32 lifetime;

    if (hData->state == 0) {
        return;
    }
    if (hData->pendingRestartSteps == 0) {
        dds3ParticleFuncTable[hData->type].update(hData);
    } else {
        for (step = 0; step < hData->pendingRestartSteps; step++) {
            dds3ParticleFuncTable[hData->type].update(hData);
        }
        hData->pendingRestartSteps = 0;
    }
    billboard = hData->hBillboard;
    record = hData->buffer->pPhysics;
    particleCount = hData->particleCount;
    lifetime = SDF_FRAMES(hData->lifetimeFrames);
    dds3BillboardSetScale(billboard, hData->billboardScale, hData->billboardScaleY);
    if (particleCount > 0) {
        step = particleCount;
        do {
            s32 age = record->age;

            if (age < lifetime && age >= 0) {
                SDF_VU0_COPYVECTOR(billboard, record->position);
                billboard->abgr = record->color;
                dds3BillboardDraw(billboard);
            }
            step--;
            record++;
        } while (step != 0);
    }
    SubmitKindDrawing(&hData->kindState);
}
