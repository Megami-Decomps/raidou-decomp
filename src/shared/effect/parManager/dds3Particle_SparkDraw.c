#include "shared/effect/parManager/decls.h"

/* Spark launch initialization retains an s4/s5 RNG-address allocation residual. */
/* Scale velocity, add its stored vertical acceleration, and advance position.
 * Only mode zero transforms the displacement through the emitter matrix. */
void dds3Particle_SparkDraw(dds3Particle_Spark_t3 *hData) {
    parSparkPacket_t *packet = (parSparkPacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    u32 mode;
    sdfVector4_t positionDelta;
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
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    packetCount = hData->head.particleCount;
    repeatEnabled = hData->loop;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SparkBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->vel[1] = packet->vel[1] * velocityScale + SDF_FRAME_STEP(packet->verticalAcceleration);
            packet->vel[0] *= velocityScale;
            packet->vel[2] *= velocityScale;
            SDF_VU0_LOADVECTOR(packet->vel);
            if (mode == 0) {
                SDF_VU0_APPLYMATRIX();
            }
            SDF_VU0_STOREVECTOR(&positionDelta);
            packet->pos[0] += SDF_FRAME_STEP(positionDelta.x);
            packet->pos[1] += SDF_FRAME_STEP(positionDelta.y);
            packet->pos[2] += SDF_FRAME_STEP(positionDelta.z);
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
