#include "common.h"
#include "sdk_libc.h"
#include "sbtlWork.h"
#include "ps2asm.h"
#include "sdfQuat.h"
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

INCLUDE_ASM(const s32, "sbtlSub", sbtlQuatFromVector);

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetUnitDeg);

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

INCLUDE_ASM(const s32, "sbtlSub", sbtlSetRotateDeg);

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetRotateDegX);

INCLUDE_ASM(const s32, "sbtlSub", sbtlCheckCrossBox);

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

INCLUDE_ASM(const s32, "sbtlSub", sbtlCheckCrossCylinder_Circle);

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

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetRadian_X);

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

INCLUDE_RODATA(const s32, "sbtlSub", sbtlGetRadian_XTable);

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

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetQuat);

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetNextPos);

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetPosforDistance);

INCLUDE_ASM(const s32, "sbtlSub", sbtlSetPushPos);

INCLUDE_ASM(const s32, "sbtlSub", sbtlSetPullPos);

INCLUDE_ASM(const s32, "sbtlSub", sbtlCheckWorldtoScreen);
