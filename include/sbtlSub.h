#ifndef SBTLSUB_H
#define SBTLSUB_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct SBTLDMG_CYLINDERWORK;
struct SBTLUNIT_WORK;

void sbtlAddUnitDeg(struct SBTLUNIT_WORK *wk, f32 deg);
s32 sbtlCheckCrossBox(const sdfVector4_t *vector1, f32 r1, u8 side, const sdfVector4_t *vector2, f32 width, f32 height);
s32 sbtlCheckCrossCircle(const sdfVector4_t *vector1, f32 r1, const sdfVector4_t *vector2, f32 r2);
s32 sbtlCheckCrossCircle_Rot(const sdfVector4_t *vector1, const sdfVector4_t *rot1, f32 r1, const sdfVector4_t *vector2, f32 r2, f32 area);
s32 sbtlCheckCrossCylinder_Circle(const sdfVector4_t *pos1, const sdfVector4_t *rot1, struct SBTLDMG_CYLINDERWORK *wk, const sdfVector4_t *pos2, f32 area);
s32 sbtlCheckInner(const sdfVector4_t *pos0, const sdfVector4_t *pos1, const sdfVector4_t *pos2, f32 rad);
s32 sbtlCheckPos_Right(const sdfVector4_t *t1, const sdfVector4_t *rot, const sdfVector4_t *t2);
s32 sbtlGetNextPos(struct SBTLUNIT_WORK *wk);
s32 sbtlGetPosforDistance(struct SBTLUNIT_WORK *wk, f32 distance);
s32 sbtlGetQuat(const sdfVector4_t *t1, const sdfVector4_t *t2);
void sbtlGetQuatFromMatrix(const sdfMatrix4_t *mat);
s32 sbtlGetRadian_X(const sdfVector4_t *t1, const sdfVector4_t *rot, const sdfVector4_t *rot2, const sdfVector4_t *t2, f32 per);
f32 sbtlGetChorikiDeg(const sdfVector4_t *pos, const sdfVector4_t *rot, const sdfVector4_t *target, f32 per);
s32 sbtlGetRotateDeg(const sdfVector4_t *rot, f32 deg);
s32 sbtlGetRotateDegX(const sdfVector4_t *rot, f32 deg);
f32 sbtlGetUnitDeg(const sdfVector4_t *rot);
f32 sbtlGetVerticalLength(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC);
s32 sbtlGetUnitHelperMatrix(struct SBTLUNIT_WORK *wk, SBTLUNIT_MARK mark);
s32 sbtlGetUnitHelperPos(struct SBTLUNIT_WORK *wk, SBTLUNIT_MARK mark);
void sbtlQuatFromVector(sdfVector4_t *qt, f32 x, f32 y, f32 z, f32 rad);
s32 sbtlSetPullPos(struct SBTLUNIT_WORK *wk1, f32 r1, struct SBTLUNIT_WORK *wk2, f32 r2, f32 ground);
s32 sbtlSetPushPos(struct SBTLUNIT_WORK *wk1, f32 r1, struct SBTLUNIT_WORK *wk2, f32 r2, f32 ground);
s32 SetVibe(s32 in, s32 life, s32 out, s32 power);
s32 sbtlSetVibeFadeIn(s32 index, u8 power, s32 frame);
s32 sbtlSetVibeFadeOut(s32 index, u8 power, s32 frame);
void sbtlSetRotateDeg(struct SBTLUNIT_WORK *wk);

#ifdef VERSION_RAIDOU2
extern const char sbtlStrWkNull2[];
extern const char sbtlStrWkModelNull[];
#endif

#endif
