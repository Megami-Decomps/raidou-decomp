#ifndef EVTSTAGE_H
#define EVTSTAGE_H

#include "common.h"
#include "basic.h"
#include "libsdf.h"

s32 evtCheckPathStat(dds3Basic_t *obj);
void evtEffectLoopControl(dds3Basic_t *eff, s32 sw);
void evtFreeAllUnit(void);
void evtKillFieldScript(void);
void evtFreeStage(void);
s32 evtLoadArea(s32 fld, s32 area);
s32 evtLoadArea_FromMemory(s32 fld, s32 area, saddr_t pF1, saddr_t pF2, const sdfTextureOffsetListHeader_t *pTB, s32 pBF);
void evtPathLoop(dds3Basic_t *basic, s32 sw);
void evtPathStop(dds3Basic_t *basic);
void evtPathStart(dds3Basic_t *basic);
void evtSetUnitPos(u32 targetKey, u32 sourceKey);
s32 evtStageAnimeControl(s32 type, s32 mode);
void evtSetLightBustup(s32 unitID, s32 frame, s32 lightNo);
s32 evtSetRegistUnit_evt(s32 major, s32 minor);
s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);

#include "enums.h"
#include "sdf_types.h"

struct BUSTUP_POS_TBL_t;

s32 evtGetDefaultPosBustUp(MODELTYPE major, struct BUSTUP_POS_TBL_t *tbl, s32 type, sdfVector4_t *pos, sdfVector4_t *rot, sdfVector4_t *defpos);
s32 evtGetDrawBustupNums(void);
s32 evtLoadUnit(s32 major, s32 minor);
s32 evtLoadUnitBustUp(MODELTYPE major, s32 minor, s32 type);
s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);

#endif
