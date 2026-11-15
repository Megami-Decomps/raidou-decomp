#ifndef SBTLPLAYER_H
#define SBTLPLAYER_H

#include "common.h"

struct SBTLUNIT_WORK;

s32 NormalProc(struct SBTLUNIT_WORK *wk);
s32 sbtlBtlDrawProcessSub(void);
s32 sbtlCheckBit(u32, s32 bit);
s32 sbtlClearEffHitData(struct SBTLUNIT_WORK *wk);
s32 sbtlGetRandomTarget(struct SBTLUNIT_WORK *wk, s32 flg, s32 bNewTgt);
s32 sbtlNormalProcNakamaSub(struct SBTLUNIT_WORK *wk);
void sbtlResetColorFlag(void);
void sbtlSetDevilID_EnemyRot(s32 _DevilID, f32 _Dir);

#endif
