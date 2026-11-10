#include "shared/effect/effPCPScatter/decls.h"

void effPCPHamaCardSphereDraw(effPCPHamaCardSphere_tag *hData) {
    sdfVector4_t axis;
    sdfVector4_t next;
    sdfVector4_t position;
    sdfVector4_t duplicatePosition;
    sdfVector4_t direction;
    sdfVector4_t radial;
    sdfVector4_t halfWidth;
    sdfVector4_t halfHeight;
    sdfVector4_t side;
    s32 respawn;
    s32 pulse;
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
    effPCPHamaCardSpherePart_tag *particle;
    f32 pulseAngle;
    f32 angleDamping;

    i = 0;
    duration = SDF_FRAMES(hData->param.life);
    endAge = SDF_FRAMES(hData->param.wait) + duration;
    duplicates = hData->pAttach != NULL;
    count = hData->param.number;
    VEC3_SPLAT(&halfWidth.x, hData->param.tex_w * 0.5f);
    VEC3_SPLAT(&halfHeight.x, hData->param.tex_h * 0.5f);
    particle = hData->pPart;
    respawn = hData->param.loop;
    pulse = hData->param.vibrate;
    pulseAngle = hData->param.vib_power;
    fadeIn = SDF_FRAMES(hData->param.fade_in);
    fadeOut = SDF_FRAMES(hData->param.fade_out);
    angleDamping = SDF_FRAME_DECAY(hData->param.roll_a);
    duplicateStart = SDF_FRAMES(hData->param.attach_f);
    perGroup = hData->param.attach_i;
    baseColor = hData->abgr;

    for (; i < count; i++, particle++) {
        s32 age = particle->frame;
        u128 *vertices = dds3ScatterPolyListGetVertex(hData->hPoly, i);
        u32 *colors = dds3ScatterPolyListGetVC(hData->hPoly, i);
        if (age == 0) {
            effPCPHamaCardSpherePartInit(hData, i);
        } else if (age > 0) {
            f32 factor = 0.0f;
            s32 moving;
            f32 angle;
            f32 radius;
            f32 angleStep;
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
            moving = age < duration;
            color = effColorBlend(baseColor & 0xFFFFFF, baseColor, factor);
            angle = particle->radian;
            radius = particle->radius;
            angleStep = particle->roll_s;
            if (!moving && pulse) {
                if (SDF_FRAME_TICK(age) == SDF_FRAME_TICK(duration)) {
                    angle += pulseAngle;
                } else if (SDF_FRAME_TICK(age) == SDF_FRAME_TICK(duration) + 1) {
                    angle -= pulseAngle;
                }
            }
            axis.x = particle->axis[0];
            axis.y = particle->axis[1];
            axis.z = particle->axis[2];
            SetRotateMatrixAxis(angle, (const void *)&axis);
            position.x = radius;
            position.y = 0.0f;
            position.z = 0.0f;
            SDF_VU0_LOADVECTOR(&position);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR_NOMEM(&position);
            SDF_VU0_COPYVECTOR_A_TO_C();
            for (j = 0; j < 3; j++, vertices++, colors++) {
                colors[0] = color;
                colors[3] = color;
                angle += sdfAtan(halfWidth.x / radius);
                SetRotateMatrixAxis(angle, (const void *)&axis);
                next.x = radius;
                next.y = 0.0f;
                next.z = 0.0f;
                SDF_VU0_LOADVECTOR(&next);
                SDF_VU0_APPLYMATRIX();
                SDF_VU0_STOREVECTOR_NOMEM(&next);
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
                SDF_VU0_STOREVECTOR_NOMEM(&radial);
                radial.y = 0.0f;
                SDF_VU0_LOADVECTOR(&direction);
                SDF_VU0_LOADVECTOR_B(&radial);
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_NORMALIZE();
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
            if (moving) {
                particle->radius += SDF_FRAME_STEP(particle->radius_s);
                particle->radian += SDF_FRAME_STEP(angleStep);
                particle->roll_s = angleStep * angleDamping;
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
            SDF_VU0_LOADVECTOR(hData->param.pos);
            SDF_VU0_LOADVECTOR_B(&duplicatePosition);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(&duplicatePosition);
            dds3EffectPartSetPosition(hData->pAttach[group], &duplicatePosition.x);
            dds3EffectPartDraw(hData->pAttach[group]);
        }
        particle->frame++;
    }
    hData->hPoly->origin[0] = hData->param.pos[0];
    hData->hPoly->origin[1] = hData->param.pos[1];
    hData->hPoly->origin[2] = hData->param.pos[2];
    dds3ScatterPolyListDraw(hData->hPoly);
}
