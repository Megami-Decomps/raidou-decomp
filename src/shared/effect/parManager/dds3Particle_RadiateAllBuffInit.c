#include "shared/effect/parManager/decls.h"

/* Spawn each packet, then overwrite its tag with a zero-based, period-grouped schedule. */
void dds3Particle_RadiateAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = 0;
    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            dds3Particle_RadiateBuffInit((dds3Particle_Radiate_t3 *)hData, packetIndex);
            packetCursor->age = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % hData->tailWords[0] == 0) {
                packetTag = packetTag - SDF_FRAMES(hData->decayStep);
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < hData->head.particleCount);
    }
}
