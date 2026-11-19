#include "common.h"
#include "sdk_libc.h"
#include "sbtlWork.h"
#include "ps2asm.h"
#include "sdfQuat.h"
#include "sdfMath.h"
#include "sdfVu0.h"
#include "kernel_draw.h"
#include "sbtlSub.h"
#include "dds3PadManager.h"
#include "sBtlUnit.h"
#include "event.h"
#include "sdfModel.h"
#include "sBtlScript.h"
#include "fpu.h"

s32 sbtlSetVibeFadeIn(s32 index, u8 power, s32 frame) {
    wk_sbtl->vibe_startf[index] = frame;
    wk_sbtl->vibe_frame[index] = frame;
    wk_sbtl->vibe_power[index] = power;
    wk_sbtl->vibe_type[index] = 1;
    return 1;
}

s32 sbtlSetVibeFadeOut(s32 index, u8 power, s32 frame) {
    wk_sbtl->vibe_startf[index] = frame;
    wk_sbtl->vibe_frame[index] = frame;
    wk_sbtl->vibe_power[index] = power;
    wk_sbtl->vibe_type[index] = 0;
    return 1;
}

void sbtlStopVibe(void) {
    wk_sbtl->vibe_startf[0] = 0;
    wk_sbtl->vibe_frame[0] = 0;
    wk_sbtl->vibe_power[0] = 0;
    wk_sbtl->vibe_type[0] = 0;
    wk_sbtl->vibe_startf[1] = 0;
    wk_sbtl->vibe_frame[1] = 0;
    wk_sbtl->vibe_power[1] = 0;
    wk_sbtl->vibe_type[1] = 0;
    dds3PadActAllStop();
}

void sbtlUpdateVibe(void) {
    s32 i;
    f32 rate;

    for (i = 0; i < 2; i++) {
        if (wk_sbtl->vibe_power[i] == 0) {
            continue;
        }
        if (wk_sbtl->vibe_frame[i] > 0 && wk_sbtl->vibe_startf[i] > 0) {
            rate = (f32)wk_sbtl->vibe_frame[i] / (f32)wk_sbtl->vibe_startf[i];
            if (wk_sbtl->vibe_type[i]) {
                rate = 1.0f - rate;
            }
            dds3PadAct(i, wk_sbtl->vibe_power[i] * rate, 100);
            wk_sbtl->vibe_frame[i]--;
        } else if (wk_sbtl->vibe_type[i] == 1) {
            dds3PadAct(i, wk_sbtl->vibe_power[i], 100);
        } else {
            dds3PadAct(i, 0, 0);
            wk_sbtl->vibe_startf[i] = 0;
        }
    }
}

s32 SetVibe(s32 in, s32 life, s32 out, s32 power) {
    s32 i;

    sbtlStopVibe();
    for (i = 0; i < 2; i++) {
        wk_sbtl->vibe[i].in = in;
        wk_sbtl->vibe[i].out = out;
        wk_sbtl->vibe[i].life = life;
        wk_sbtl->vibe[i].power = power;
        wk_sbtl->vibe[i].count = 0;
    }
    return 1;
}

s32 sbtlCalcVibe(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (wk_sbtl->vibe[i].life < wk_sbtl->vibe[i].out) {
            return 0;
        }
        if (wk_sbtl->vibe[i].count == 0) {
            sbtlSetVibeFadeIn(i, wk_sbtl->vibe[i].power, wk_sbtl->vibe[i].in);
        }
        if (wk_sbtl->vibe[i].life > 0 && wk_sbtl->vibe[i].count == wk_sbtl->vibe[i].life - wk_sbtl->vibe[i].out) {
            sbtlSetVibeFadeOut(i, wk_sbtl->vibe[i].power, wk_sbtl->vibe[i].out);
        }
        wk_sbtl->vibe[i].count++;
        wk_sbtl->vibe[i].count = (wk_sbtl->vibe[i].count <= 0) ? 0 : (wk_sbtl->vibe[i].count > 0x7FFF) ? 0x7FFF : wk_sbtl->vibe[i].count;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetUnitHelperPos);

s32 sbtlGetUnitHelperMatrix(struct SBTLUNIT_WORK *wk, SBTLUNIT_MARK mark) {
    if (GetUnitHelperMatrix(wk, mark)) {
        return 1;
    }
    if (wk->hModel != NULL && (!(wk->flg & 0x40) || !wk->changeflg)) {
        sdfModelEvalTm(wk->hEveUnit->owner->inner);
        sdfModelGetMarkPosMatrix_sdfModel(wk->hEveUnit->owner->inner, mark);
        return 1;
    }
    SDF_VU0_LOADIDENTITY();
    return 1;
}

/* body: matched in raidou @ 780f3f57: src/raidou1/sbtlSub.c:133, found, same function 1.00 (words 155/155, calls 1.00, shape 0.99) */
void sbtlGetQuatFromMatrix(const sdfMatrix4_t *mat) {
    f32 q[4];
    f32 trace;
    f32 scale;
    s32 i;
    u8 j;
    u8 k;
    const f32 *m;

    m = mat->m;
    trace = m[0] + m[5] + m[10] + 1.0f;
    if (trace > 1.0f) {
        scale = 2.0f * fsqrtf(trace);
        q[3] = -(scale * 0.25f);
        q[0] = (m[6] - m[9]) / scale;
        q[1] = (m[8] - m[2]) / scale;
        q[2] = (m[1] - m[4]) / scale;
    } else {
        i = m[0] > m[5] ? 0 : 1;
        if (m[i * 4 + i] < m[10]) {
            i = 2;
        }
        j = (i + 1) % 3;
        k = (j + 1) % 3;
        scale = 2.0f * fsqrtf(m[i * 4 + i] - m[j * 4 + j] - m[k * 4 + k] + 1.0f);
        if (scale != 0.0f) {
            q[i] = scale * 0.25f;
            q[j] = (m[i * 4 + j] + m[j * 4 + i]) / scale;
            q[k] = (m[i * 4 + k] + m[k * 4 + i]) / scale;
            q[3] = -((m[j * 4 + k] - m[k * 4 + j]) / scale);
        } else {
            q[i] = 1.0f;
            q[j] = 0.0f;
            q[k] = 0.0f;
            q[3] = 0.0f;
        }
    }
    SDF_VU0_LOADVECTOR(q);
    Normalize();
}

void sbtlQuatFromVector(sdfVector4_t *qt, f32 x, f32 y, f32 z, f32 rad)
{
    sdfVector4_t axis;
    f32 s;
    f32 c;

    axis.x = x;
    axis.y = y;
    axis.z = z;
    axis.w = 1.0f;
    s = fsqrtf((1.0f - sdfCos(rad)) * 0.5f);
    c = fsqrtf((sdfCos(rad) + 1.0f) * 0.5f);
    SDF_VU0_LOADVECTOR(&axis);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&axis);
    qt->w = c;
    qt->x = axis.x * s;
    qt->y = axis.y * s;
    qt->z = axis.z * s;
}

f32 sbtlGetUnitDeg(const sdfVector4_t *rot)
{
    SDF_VU0_LOADVECTOR(rot);
    return EulerAngleAxisY() * 57.29577637f;
}

void sbtlAddUnitDeg(struct SBTLUNIT_WORK *wk, f32 deg) {
    wk->angleY += deg;
    while (wk->angleY >= 360.0f || wk->angleY < 0.0f) {
        if (wk->angleY >= 360.0f) {
            wk->angleY -= 360.0f;
        }
        if (wk->angleY < 0.0f) {
            wk->angleY += 360.0f;
        }
    }
}

s32 sbtlGetRotateDeg(const sdfVector4_t *rot, f32 deg) {
    f32 axis[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

    SDF_VU0_LOADVECTOR(axis);
    FromRotation(-deg * 0.017453294f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(rot);
    Mult();
    return 1;
}

void sbtlSetRotateDeg(struct SBTLUNIT_WORK *wk)
{
    SDF_VU0_LOADVECTOR(&wk->rot);
    wk->angleY = -EulerAngleAxisY() * 57.29577637f;
}

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetRotateDegX);

s32 sbtlCheckCrossBox(const sdfVector4_t *vector1, f32 r1, u8 side, const sdfVector4_t *vector2, f32 width, f32 height)
{
    sdfVector4_t pos;
    sdfVector4_t box;
    sdfVector4_t top;
    s32 hit;
    f32 len;

    SDF_VU0_LOADVECTOR(vector1);
    SDF_VU0_STOREVECTOR(&pos);
    SDF_VU0_LOADVECTOR(vector2);
    SDF_VU0_STOREVECTOR(&box);
    SDF_VU0_STOREVECTOR(&top);
    hit = 0;
    top.y -= height;
    if (box.y >= pos.y && sbtlCheckCrossCircle(&pos, r1, &top, width)) {
        if (top.y - width <= pos.y) {
            hit = 1;
        } else if (sbtlCheckCrossCircle(&pos, r1, &box, width)) {
            hit = 2;
        }
    } else if (sbtlCheckCrossCircle(&pos, r1, &box, width) && pos.y == box.y) {
        hit = 2;
    }
    switch (hit) {
    case 1:
        pos.y = 0.0f;
        box.y = 0.0f;
        SDF_VU0_LOADVECTOR(&pos);
        SDF_VU0_LOADVECTOR_B(&box);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(len);
        if (len < width + r1 * 0.5f) {
            return 0;
        }
        break;
    case 2:
        pos.y = 0.0f;
        box.y = 0.0f;
        SDF_VU0_LOADVECTOR(&pos);
        SDF_VU0_LOADVECTOR_B(&box);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(len);
        if (len < width + r1) {
            return 1;
        }
        break;
    }
    return 3;
}

s32 sbtlCheckCrossCircle(const sdfVector4_t *vector1, f32 r1, const sdfVector4_t *vector2, f32 r2) {
    f32 len;

    SDF_VU0_LOADVECTOR(vector1);
    SDF_VU0_LOADVECTOR_B(vector2);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(len);
    if (len <= r1 + r2) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlSub", sbtlCheckCrossCircle_Rot);

s32 sbtlCheckCrossCylinder_Circle(const sdfVector4_t *pos1, const sdfVector4_t *rot1, struct SBTLDMG_CYLINDERWORK *wk, const sdfVector4_t *pos2, f32 area)
{
    sdfVector4_t vertexA;
    sdfVector4_t vertexB;
    sdfVector4_t vertexC;
    sdfVector4_t ofs;
    sdfMatrix4_t mat;
    sdfMatrix4_t *pmat;
    sdfVector4_t *pB;

    memset(&ofs, 0, sizeof(ofs));
    ofs.y = -1.0f;
    SDF_VU0_LOADVECTOR(pos1);
    SDF_VU0_STOREVECTOR(&vertexA);
    SDF_VU0_LOADVECTOR(pos2);
    SDF_VU0_STOREVECTOR(&vertexC);
    SDF_VU0_LOADVECTOR(rot1);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SetRotateMatrixX(wk->xrad * 0.0174532925f);
    pmat = &mat;
    SDF_VU0_STOREMATRIX_RWADDR(pmat);
    sbtlGetQuatFromMatrix(&mat);
    Mult();
    ToMatrix();
    SDF_VU0_LOADVECTOR(&ofs);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_SCALEVECTOR(wk->height);
    SDF_VU0_LOADVECTOR_B(&vertexA);
    SDF_VU0_ADDVECTOR();
    pB = &vertexB;
    SDF_VU0_STOREVECTOR_RWADDR(pB);
    if (sbtlGetVerticalLength(&vertexA.x, &vertexB.x, &vertexC.x) <= area) {
        if (sbtlCheckInner(&vertexA, &vertexB, &vertexC, 0.0f) || sbtlCheckCrossCircle(&vertexA, area - wk->area_r, &vertexC, 1.0f)) {
            if (sbtlCheckInner(&vertexB, &vertexA, &vertexC, 0.0f) || sbtlCheckCrossCircle(&vertexB, area - wk->area_r, &vertexC, 1.0f)) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA(const s32, "sbtlSub", sbtlGetRotateDegXTable);

f32 sbtlGetChorikiDeg(const sdfVector4_t *pos, const sdfVector4_t *rot, const sdfVector4_t *target, f32 per) {
    sdfVector4_t p0;
    sdfVector4_t p1;
    sdfVector4_t dir;
    sdfVector4_t front;
    f32 up[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    f32 ofs[4] = { 0.0f, 0.0f, -1.0f, 1.0f };
    f32 dot;
    f32 deg;
    f32 rdeg;

    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&p0);
    p0.y = 0.0f;
    SDF_VU0_LOADVECTOR(target);
    SDF_VU0_STOREVECTOR(&p1);
    p1.y = 0.0f;
    SDF_VU0_LOADVECTOR_B(ofs);
    SDF_VU0_LOADVECTOR(rot);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&p0);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(&p0);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&front);
    SDF_VU0_LOADVECTOR_B(&p0);
    SDF_VU0_LOADVECTOR(&p1);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    SDF_VU0_LOADVECTOR(&front);
    SDF_VU0_LOADVECTOR_B(&dir);
    SDF_VU0_INNERPRODUCT(deg);
    if (deg > 1.0f) {
        SDF_VU0_LOADVECTOR(rot);
        return 0.0f;
    }
    if (deg < -1.0f) {
        SDF_VU0_LOADVECTOR(rot);
        return 180.0f;
    }
    deg = acosf(deg);
    SDF_VU0_LOADVECTOR(up);
    FromRotation(-1.5707959f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(rot);
    Mult();
    SDF_VU0_LOADVECTOR_B(ofs);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&p0);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(&p0);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(&dir);
    SDF_VU0_INNERPRODUCT(dot);
    if (dot < 0.0f) {
        deg = -deg;
    }
    rdeg = deg * 57.29577637f * per;
    sbtlGetRotateDeg(rot, rdeg);
    return rdeg;
}

s32 sbtlGetRadian_X(const sdfVector4_t *t1, const sdfVector4_t *rot, const sdfVector4_t *rot2, const sdfVector4_t *t2, f32 per) {
    sdfVector4_t p0;
    sdfVector4_t p1;
    sdfVector4_t dir;
    sdfVector4_t front;
    sdfVector4_t quat;
    sdfVector4_t axis = { 1.0f, 0.0f, 0.0f, 1.0f };
    sdfVector4_t ofs;
    sdfVector4_t ident;
    f32 deg;
    f32 dot;
    f32 side;

    memset(&ofs, 0, sizeof(ofs));
    ofs.z = -1.0f;
    memset(&ident, 0, sizeof(ident));
    ident.w = 1.0f;
    SDF_VU0_LOADVECTOR(t1);
    SDF_VU0_STOREVECTOR(&p0);
    SDF_VU0_LOADVECTOR(t2);
    SDF_VU0_STOREVECTOR(&p1);
    p0.x = 0.0f;
    p1.x = 0.0f;
    SDF_VU0_LOADVECTOR_B(&ofs);
    SDF_VU0_LOADVECTOR(rot2);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(rot);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&p0);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(&p0);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&front);
    SDF_VU0_LOADVECTOR_B(&p0);
    SDF_VU0_LOADVECTOR(&p1);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    SDF_VU0_LOADVECTOR(&front);
    SDF_VU0_LOADVECTOR_B(&dir);
    SDF_VU0_INNERPRODUCT(dot);
    if (dot > 1.0f || dot < -1.0f) {
        SDF_VU0_LOADVECTOR(&ident);
        return 0;
    }
    deg = acosf(dot);
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(-1.5707959f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(rot2);
    Mult();
    SDF_VU0_LOADVECTOR_B(&ofs);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(rot);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&p0);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(&p0);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(&dir);
    SDF_VU0_INNERPRODUCT(side);
    if (side < 0.0f) {
        deg = -deg;
    }
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(-(deg * per));
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(rot);
    Mult();
    SDF_VU0_STOREVECTOR(&quat);
    return 1;
}

s32 sbtlCheckInner(const sdfVector4_t *pos0, const sdfVector4_t *pos1, const sdfVector4_t *pos2, f32 rad) {
    sdfVector4_t v1;
    sdfVector4_t v2;
    f32 dot;

    SDF_VU0_LOADVECTOR(pos1);
    SDF_VU0_LOADVECTOR_B(pos0);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&v1);
    SDF_VU0_LOADVECTOR(pos2);
    SDF_VU0_LOADVECTOR_B(pos0);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&v2);
    SDF_VU0_LOADVECTOR(&v1);
    SDF_VU0_LOADVECTOR_B(&v2);
    SDF_VU0_INNERPRODUCT(dot);
    if (rad <= dot) {
        return 1;
    }
    return 0;
}

s32 sbtlCheckPos_Right(const sdfVector4_t *t1, const sdfVector4_t *rot, const sdfVector4_t *t2) {
    sdfVector4_t pos1;
    sdfVector4_t pos2;
    sdfVector4_t dir;
    f32 axis[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    sdfVector4_t ofs;
    f32 dot;

    memset(&ofs, 0, sizeof(ofs));
    ofs.z = -1.0f;
    SDF_VU0_LOADVECTOR(t1);
    SDF_VU0_STOREVECTOR(&pos1);
    pos1.y = 0.0f;
    SDF_VU0_LOADVECTOR(t2);
    SDF_VU0_STOREVECTOR(&pos2);
    pos2.y = 0.0f;
    SDF_VU0_LOADVECTOR_B(&ofs);
    SDF_VU0_LOADVECTOR(rot);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&pos1);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(&pos1);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(&pos1);
    SDF_VU0_LOADVECTOR(&pos2);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    SDF_VU0_LOADVECTOR(axis);
    FromRotation(-1.5707959f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(rot);
    Mult();
    SDF_VU0_LOADVECTOR_B(&ofs);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&pos1);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(&pos1);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(&dir);
    SDF_VU0_INNERPRODUCT(dot);
    if (dot < 0.0f) {
        return 0;
    }
    return 1;
}

s32 sbtlGetQuat(const sdfVector4_t *t1, const sdfVector4_t *t2)
{
    sdfVector4_t q;
    sdfVector4_t dir;

    memset(&q, 0, sizeof(q));
    q.w = 1.0f;
    SDF_VU0_LOADVECTOR(t2);
    SDF_VU0_LOADVECTOR_B(t1);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    if (dir.x != 0.0f || dir.z != 0.0f) {
        FromEuler(0.0f, sdfAtan2(dir.x, dir.z), 0.0f);
        return 1;
    }
    SDF_VU0_LOADVECTOR(&q);
    return 0;
}

s32 sbtlGetNextPos(struct SBTLUNIT_WORK *wk)
{
    sdfVector4_t ofs;

    memset(&ofs, 0, sizeof(ofs));
    ofs.x = -wk->mv_spd.x;
    ofs.z = -wk->mv_spd.z;
    SDF_VU0_LOADVECTOR_B(&ofs);
    SDF_VU0_LOADVECTOR(&wk->rot);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&wk->pos);
    SDF_VU0_ADDVECTOR();
    return 1;
}

s32 sbtlGetPosforDistance(struct SBTLUNIT_WORK *wk, f32 distance)
{
    sdfVector4_t ofs;

    memset(&ofs, 0, sizeof(ofs));
    ofs.z = -distance;
    SDF_VU0_LOADVECTOR_B(&ofs);
    SDF_VU0_LOADVECTOR(&wk->rot);
    ToMatrix();
    SDF_VU0_APPLYMATRIX_B();
    SDF_VU0_LOADVECTOR(&wk->pos);
    SDF_VU0_ADDVECTOR();
    return 1;
}

INCLUDE_ASM(const s32, "sbtlSub", sbtlSetPushPos);

INCLUDE_ASM(const s32, "sbtlSub", sbtlSetPullPos);

s32 sbtlCheckWorldtoScreen(const sdfVector4_t *pos)
{
    f32 len;
    f32 dot;
    s32 ret;

    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(len);
    if (len > 16.0) {
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADMATRIX(&CameraTM);
        SDF_VU0_COPYMATRIX_A_TO_B();
        ReverseMatrix();
        SDF_VU0_GET_ROW2();
        SDF_VU0_NORMALIZE();
        SDF_VU0_INNERPRODUCT(dot);
        ret = 0;
        if (!(dot <= 0.5f)) {
            ret = 1;
        }
        return ret;
    }
    return 0;
}
