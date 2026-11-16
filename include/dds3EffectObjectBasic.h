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

#include "effManager.h"
#include "sdf_types.h"

typedef struct dds3EffectObjectBasicWorkData_t {
    dds3ObjectBaseControlWorkData_t *pObjectBaseControlWorkData;
    u32 stat;
    s32 type;
    void *handle;
    u32 linkpos[4]; /* sdfVector4_t in the IDB, written as words here */
    dds3Basic_t *linkbasic;
    u16 linkindex;
    u16 linkbasictype;
    void *para;
    struct effEveBEDCut_t *cuth;
    s32 reserve1;
    s32 reserve2;
    s32 reserve3;
    struct fldResourceEffect_t *pFldEx;
    sdfVector4_t FldExBasePos;
} dds3EffectObjectBasicWorkData_t;

dds3Basic_t *AddEPLEffect(effHandle_t *eff, const sdfVector4_t *pos, const sdfVector4_t *rot);
dds3Basic_t *dds3EffectObjectBasicAddD3P2(void *effdata, const sdfVector4_t *pos, const sdfVector4_t *rot);
dds3Basic_t *dds3EffectObjectBasicAddEPL(void *effdata, const sdfVector4_t *pos, const sdfVector4_t *rot);
s32 dds3EffectObjectBasicMainProcedure(dds3Basic_t *aPtr);

#endif
