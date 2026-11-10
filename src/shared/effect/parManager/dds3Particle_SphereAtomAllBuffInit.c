#include "shared/effect/parManager/decls.h"

/* Initialize each packet's native tag and its template-selected uniform scale. */
void dds3Particle_SphereAtomAllBuffInit(effTemplatePacketList_t *hData) {
    s32 packetIndex = 0;
    effTemplateScaleRecord_t *packetCursor = (effTemplateScaleRecord_t *)hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->initialTag = EFF_PACKET_INITIAL_TAG;
            packetCursor->scale = hData->recordScale;
            packetIndex++;
            packetCursor++;
        } while ((u32)packetIndex < hData->head.particleCount);
    }
}
