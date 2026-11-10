#include "shared/effect/effPCPScatter/decls.h"

/* vu0 routine: each particle sweeps three ribbon quads around its tilt angle,
 * and the tilt reverses once it passes half its stored span. */
void effPCPHamaCardScatterDraw(effPCPHamaCardScatter_tag *hData) {
    sdfVector4_t next;
    sdfVector4_t position;
    sdfVector4_t duplicatePosition;
    sdfVector4_t direction;
    sdfVector4_t axis;
    sdfVector4_t halfWidth;
    sdfVector4_t halfHeight;
    sdfVector4_t side;
    s32 respawn;
    s32 duplicates;
    u32 i;
    u32 count;
    u32 fadeIn;
    u32 fadeOut;
    u32 color;
    u32 baseColor;
    u32 perGroup;
    s32 duration;
    s32 endAge;
    s32 duplicateStart;
    effPCPHamaCardScatterPart_tag *particle;

    i = 0;
    duration = SDF_FRAMES(hData->param.duration);
    endAge = SDF_FRAMES(hData->param.fadeDuration) + duration;
    duplicates = hData->pAttach != NULL;
    count = hData->param.particleCount;
    VEC3_SPLAT(&halfWidth.x, hData->param.width * 0.5f);
    VEC3_SPLAT(&halfHeight.x, hData->param.height * 0.5f);
    particle = hData->pPart;
    respawn = hData->param.respawn;
    fadeIn = SDF_FRAMES(hData->param.fadeIn);
    fadeOut = SDF_FRAMES(hData->param.fadeOut);
    duplicateStart = SDF_FRAMES(hData->param.duplicateStartAge);
    perGroup = hData->param.particlesPerGroup;
    baseColor = hData->abgr;

    for (; i < count; i++, particle++) {
        s32 age = particle->frame;
        u128 *vertices = dds3ScatterPolyListGetVertex(hData->hPoly, i);
        u32 *colors = dds3ScatterPolyListGetVC(hData->hPoly, i);
        if (age == 0) {
            effPCPHamaCardScatterPartInit(hData, i);
        } else if (age > 0) {
            f32 factor = 0.0f;
            f32 angle;
            f32 radius;
            f32 height;
            f32 heightStep;
            f32 tiltStep;
            u32 j;

            if (age <= endAge) {
                if (age < fadeIn && fadeIn != 0) {
                    factor = (f32)age / fadeIn;
                } else if (endAge - age <= fadeOut && fadeOut != 0) {
                    factor = (f32)(endAge - age) / fadeOut;
                } else {
                    factor = 1.0f;
                }
            }
            color = effColorBlend(baseColor & 0xFFFFFF, baseColor, factor);
            radius = particle->radius;
            height = particle->height;
            heightStep = particle->height_s;
            angle = particle->radian - sdfAtan(halfWidth.x / radius);
            tiltStep = particle->radian_s;
            axis.x = 0.0f;
            axis.y = 0.0f;
            axis.z = 1.0f;
            position.x = sdfSin(angle) * radius;
            position.y = sdfCos(angle) * radius + height;
            position.z = 0.0f;
            SDF_VU0_LOADVECTOR(&position);
            SDF_VU0_COPYVECTOR_A_TO_C();
            for (j = 0; j < 3; j++, vertices++, colors++) {
                colors[0] = color;
                colors[3] = color;
                height += heightStep;
                angle += sdfAtan(halfWidth.x / radius);
                next.x = sdfSin(angle) * radius;
                next.y = sdfCos(angle) * radius + height;
                next.z = 0.0f;
                SDF_VU0_LOADVECTOR(&next);
                SDF_VU0_LOADVECTOR_B(&position);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR_NOMEM(&direction);
                SDF_VU0_LOADVECTOR_B(&halfWidth);
                SDF_VU0_MULVECTOR();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_STOREVECTOR_NOMEM(&position);
                SDF_VU0_LOADVECTOR(&axis);
                SDF_VU0_LOADVECTOR_B(&halfHeight);
                SDF_VU0_MULVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(&side);
                SDF_VU0_LOADVECTOR(&position);
                SDF_VU0_LOADVECTOR_B(&side);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(vertices);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(vertices + 3);
                if (j == 1) {
                    SDF_VU0_COPYVECTOR(&duplicatePosition, &position);
                }
                SDF_VU0_COPYVECTOR(&position, &next);
            }
            if (age < duration) {
                f32 limit = particle->radian_w * 0.5f;
                f32 tilt = particle->radian + SDF_FRAME_STEP(tiltStep);
                f32 magnitude;

                particle->radian = tilt;
                particle->height = particle->height + SDF_FRAME_STEP(heightStep);
                magnitude = sdfFabs(tilt);
                if (limit < magnitude) {
                    particle->radian = tilt - SDF_FRAME_STEP(tiltStep);
                    particle->radian_s = -particle->radian_s;
                }
            }
        } else {
            colors[0] = 0;
            colors[1] = 0;
            colors[2] = 0;
            colors[3] = 0;
            colors[4] = 0;
            colors[5] = 0;
        }
        if (respawn && age >= endAge) {
            particle->frame = -1;
        }
        if (duplicates && age >= duplicateStart && age >= 0 && i % perGroup == 0) {
            u32 group = i / perGroup;
            dds3EffectPartSetColor(hData->pAttach[group], baseColor);
            SDF_VU0_LOADVECTOR(hData->param.origin);
            SDF_VU0_LOADVECTOR_B(&duplicatePosition);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(&duplicatePosition);
            dds3EffectPartSetPosition(hData->pAttach[group], &duplicatePosition.x);
            dds3EffectPartDraw(hData->pAttach[group]);
        }
        particle->frame++;
    }
    hData->hPoly->origin[0] = hData->param.origin[0];
    hData->hPoly->origin[1] = hData->param.origin[1];
    hData->hPoly->origin[2] = hData->param.origin[2];
    dds3ScatterPolyListDraw(hData->hPoly);
}
