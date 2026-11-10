#include "shared/effect/parManager/decls.h"

/* Stagger native tags using this variant's second tail word as the group period. */
void dds3Particle_SphereRingAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = EFF_PACKET_INITIAL_TAG;
    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % hData->tailWords[1] == 0) {
                packetTag = packetTag - SDF_FRAMES(hData->decayStep);
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < hData->head.particleCount);
    }
}
