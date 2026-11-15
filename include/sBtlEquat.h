#ifndef SBTLEQUAT_H
#define SBTLEQUAT_H

#include "common.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;

s32 CheckEnemyTarget(struct SBTLUNIT_WORK *wk);
s32 SetOrderTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlCheckCrossCircle(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos, f32 area);
s32 sbtlCheckAreaCount(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos, f32 area);
s32 sbtlCheckArea_Atk(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos, f32 area);
s32 sbtlCheckArea_Masic(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos, f32 area, s32 flg);

#endif
