#ifndef SBTLSOUND_H
#define SBTLSOUND_H

#include "common.h"
#include "sdf_types.h"

struct SBTLEFF_PACLIST;
struct SBTLUNIT_WORK;

void DevilFootSEProc(struct SBTLUNIT_WORK *wk);
void sbtlCallGetRandIntA5(const sdfVector4_t *pos, u32 seid);
u32 sbtlGetBossSE(s32 devilID, s32 motionID);
s32 sbtlLoadSkillSE(struct SBTLEFF_PACLIST *ewk);
void sbtlPlay3DSE_BaseVol(u32 no, const sdfVector4_t *pos, s32 flg);
void sbtlPlayDamageSE(const sdfVector4_t *pos, s32 damageType);
void sbtlPlaySpecialSE(const sdfVector4_t *pos, s32 specialType);

#endif
