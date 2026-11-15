#ifndef EVTSTAGE_H
#define EVTSTAGE_H

#include "common.h"
#include "basic.h"
#include "libsdf.h"

s32 evtCheckPathStat(dds3Basic_t *obj);
void evtEffectLoopControl(dds3Basic_t *eff, s32 sw);
void evtFreeAllUnit(void);
void evtFreeStage(void);
s32 evtLoadArea(s32 fld, s32 area);
s32 evtLoadArea_FromMemory(s32 fld, s32 area, saddr_t pF1, saddr_t pF2, const sdfTextureOffsetListHeader_t *pTB, s32 pBF);
void evtPathLoop(dds3Basic_t *basic, s32 sw);
void evtSetUnitPos(u32 targetKey, u32 sourceKey);
s32 evtStageAnimeControl(s32 type, s32 mode);

#endif
