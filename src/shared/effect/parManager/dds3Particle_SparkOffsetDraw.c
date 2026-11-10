#include "shared/effect/parManager/decls.h"

/* Accumulate motion in the stored offset, then add the emitter origin.
 * Only mode zero transforms the per-frame displacement through the matrix. */
void dds3Particle_SparkOffsetDraw(dds3Particle_SparkOffset_t3 *hData) {
    parSparkOffsetPacket_t *packet = (parSparkOffsetPacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    u32 mode;
    sdfVector4_t positionDelta;
    sdfVector4_t origin;
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&hData->head.sub);
    mode = hData->mode;
    if (mode == 0) {
        SDF_VU0_LOADMATRIX(hData->head.matrix);
    }
    velocityScale = SDF_FRAME_DECAY(hData->decayPct / 100.0f + 1.0f);
    completedCount = 0;
    packetCount = hData->head.particleCount;
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    repeatEnabled = hData->loop;
    SDF_VU0_COPYVECTOR(&origin, hData->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SparkOffsetBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->vel[1] = packet->vel[1] * velocityScale + SDF_FRAME_STEP(packet->offsetAcceleration);
            packet->vel[0] *= velocityScale;
            packet->vel[2] *= velocityScale;
            SDF_VU0_LOADVECTOR(packet->vel);
            if (mode == 0) {
                SDF_VU0_APPLYMATRIX();
            }
            SDF_VU0_STOREVECTOR(&positionDelta);
            packet->offsetX += SDF_FRAME_STEP(positionDelta.x);
            packet->offsetY += SDF_FRAME_STEP(positionDelta.y);
            packet->offsetZ += SDF_FRAME_STEP(positionDelta.z);
            SDF_VU0_LOADVECTOR(&packet->offsetX);
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}
