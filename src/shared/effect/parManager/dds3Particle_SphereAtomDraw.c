#include "shared/effect/parManager/decls.h"

/* Reflect moving packets at the frame's entry radius. The first record's
 * updated radius becomes the boundary on the following frame. */
void dds3Particle_SphereAtomDraw(effTemplatePacketList_t *hData) {
    sdfVector4_t origin;
    sdfVector4_t previousPosition;
    /* Retail leaves the fourth broadcast components uninitialized. The
     * XYZW operations propagate those W lanes into the packet stores. */
    sdfVector4_t reflectionScale;
    sdfVector4_t normal;
    sdfVector4_t boundaryScale;
    s32 completedCount = 0;
    s32 repeatEnabled;
    parSphereAtomPacket_t *packet = (parSphereAtomPacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 index;
    f32 radiusStep;
    f32 targetRadius;
    f32 radius;
    f32 velocityScale;

    dds3Particle_EffectInit(&hData->head.sub);
    lifetimeFrames = SDF_FRAMES((s32)hData->head.lifetimeFrames);
    targetRadius = hData->targetRadius;
    radiusStep = (targetRadius - hData->recordScale) / lifetimeFrames;
    velocityScale = SDF_FRAME_DECAY(hData->accelerationPct / 100.0f + 1.0f);
    packetCount = (s32)hData->head.particleCount;
    repeatEnabled = hData->restart;
    radius = packet->boundaryRadius;
    if (radiusStep >= 0.0f) {
        if (radius < targetRadius) {
            packet->boundaryRadius = radius + radiusStep;
            if (packet->boundaryRadius > targetRadius) {
                packet->boundaryRadius = targetRadius;
            }
        }
    } else {
        if (radius > targetRadius) {
            packet->boundaryRadius = radius + radiusStep;
            if (packet->boundaryRadius < targetRadius) {
                packet->boundaryRadius = targetRadius;
            }
        }
    }
    SDF_VU0_COPYVECTOR(&origin, hData->head.origin);
    for (index = 0; index < packetCount; index++, packet++) {
        s32 age = packet->age;
        f32 distance;
        f32 dot;
        f32 dx, dy, dz;
        f32 magnitude;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SphereAtomBuffInit(hData, index);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_COPYVECTOR(&previousPosition, packet->pos);
            }
            magnitude = packet->motionMagnitude;
            dx = packet->vel[0] * magnitude;
            dy = packet->vel[1] * magnitude;
            dz = packet->vel[2] * magnitude;
            packet->pos[0] += SDF_FRAME_STEP(dx);
            packet->pos[1] += SDF_FRAME_STEP(dy);
            packet->pos[2] += SDF_FRAME_STEP(dz);
            magnitude *= velocityScale;
            packet->motionMagnitude = magnitude;
            SDF_VU0_LOADVECTOR(packet->pos);
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_GETLENGTH(distance);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&normal);
            if (distance > radius) {
                SDF_VU0_LOADVECTOR(packet->vel);
                SDF_VU0_LOADVECTOR_B(dds3ParticleNegOneVec);
                SDF_VU0_MULVECTOR();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_LOADVECTOR(&origin);
                SDF_VU0_LOADVECTOR_B(packet->pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_NORMALIZE();
                SDF_VU0_COPYVECTOR_C_TO_B();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_INNERPRODUCT(dot);
                reflectionScale.x = dot * 2.0f;
                reflectionScale.y = dot * 2.0f;
                reflectionScale.z = dot * 2.0f;
                SDF_VU0_LOADVECTOR(&reflectionScale);
                SDF_VU0_COPYVECTOR_C_TO_B();
                SDF_VU0_MULVECTOR();
                SDF_VU0_LOADVECTOR_B(packet->vel);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(packet->vel);
                boundaryScale.x = radius;
                boundaryScale.y = radius;
                boundaryScale.z = radius;
                SDF_VU0_LOADVECTOR(&normal);
                SDF_VU0_LOADVECTOR_B(&boundaryScale);
                SDF_VU0_MULVECTOR();
                SDF_VU0_LOADVECTOR_B(&origin);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(packet->pos);
            }
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(&previousPosition);
                SDF_VU0_LOADVECTOR(packet->pos);
                dds3Particle_EffectSet(&hData->head.sub, index, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, index);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}
