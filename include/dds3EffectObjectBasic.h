#ifndef DDS3EFFECTOBJECTBASIC_H
#define DDS3EFFECTOBJECTBASIC_H

#include "common.h"
#include "basic.h"

void *dds3AddBEDEffectObjectBasic(void *file, void *pos, void *rot);
void *dds3AddBEDEffectObjectBasic2(void *data, void *pos, void *rot);
void *dds3AddMG12EffectObjectBasic(s32 type, void *file);
dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata);
dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *pBasic);
s32 dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);
s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index);
void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 s);
void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);
s32 dds3EffectObjectPresetMG12(dds3Basic_t *eff);
s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1, dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);
void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, u32 cutno);

#endif
