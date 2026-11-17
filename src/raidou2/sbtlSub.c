#include "common.h"
#include "sdk_libc.h"
#include "sbtlWork.h"
#include "ps2asm.h"
#include "sdfQuat.h"

INCLUDE_ASM(const s32, "sbtlSub", sbtlSetVibeFadeIn);

INCLUDE_ASM(const s32, "sbtlSub", sbtlSetVibeFadeOut);

INCLUDE_ASM(const s32, "sbtlSub", sbtlStopVibe);

INCLUDE_ASM(const s32, "sbtlSub", sbtlUpdateVibe);

INCLUDE_ASM(const s32, "sbtlSub", SetVibe);

INCLUDE_ASM(const s32, "sbtlSub", sbtlCalcVibe);

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetUnitHelperPos);

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetUnitHelperMatrix);

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetQuatFromMatrix);

INCLUDE_ASM(const s32, "sbtlSub", sbtlQuatFromVector);

INCLUDE_ASM(const s32, "sbtlSub", sbtlGetUnitDeg);

INCLUDE_ASM(const s32, "sbtlSub", sbtlAddUnitDeg);

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
