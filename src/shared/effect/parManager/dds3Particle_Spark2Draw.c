#include "shared/effect/parManager/decls.h"

/* Scale velocity, apply stored gravity, and transform each motion step before aging. */
void dds3Particle_Spark2Draw(dds3Particle_Spark2_t3 *hData) {
    parSparkPacket_t *packet = (parSparkPacket_t *)hData->basic.buffer->pPhysics;
    u32 subeffectKind = hData->basic.sub.type;
    sdfVector4_t positionDelta;
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&hData->basic.sub);
    SDF_VU0_LOADMATRIX(hData->basic.matrix);
    velocityScale = SDF_FRAME_DECAY(hData->decayPct / 100.0f + 1.0f);
    completedCount = 0;
    lifetimeFrames = SDF_FRAMES(hData->basic.lifetimeFrames);
    packetCount = hData->basic.particleCount;
    repeatEnabled = hData->loop;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_Spark2BuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->basic.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->basic.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->vel[0] *= velocityScale;
            packet->vel[1] = packet->vel[1] * velocityScale + SDF_FRAME_STEP(packet->verticalAcceleration);
            packet->vel[2] *= velocityScale;
            SDF_VU0_LOADVECTOR(packet->vel);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(&positionDelta);
            packet->pos[0] += SDF_FRAME_STEP(positionDelta.x);
            packet->pos[1] += SDF_FRAME_STEP(positionDelta.y);
            packet->pos[2] += SDF_FRAME_STEP(positionDelta.z);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->basic.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->basic.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->basic.active = 0;
                }
            }
        }
        packet->age = age;
    }
}
