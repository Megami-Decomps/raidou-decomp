#ifndef SBTLENCOUNT_H
#define SBTLENCOUNT_H

#include "common.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;

struct SBTL_AISYO_TBL *sbtlGetAisyotbl(s32 id);
s32 sbtlGetPointData(s32 pointID, sdfVector4_t *pos, sdfVector4_t *rot);
void sbtlSetactcounter(struct SBTLUNIT_WORK *wk);

#endif
