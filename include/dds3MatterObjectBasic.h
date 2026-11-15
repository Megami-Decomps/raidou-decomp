#ifndef DDS3MATTEROBJECTBASIC_H
#define DDS3MATTEROBJECTBASIC_H

#include "common.h"
#include "basic.h"

u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *aPtr);
void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *aPtr, u32 alpmode);

s32 MainProcedure(dds3Basic_t *aPtr);
s32 dds3MatterObjectBasicDrawProcedure(dds3Basic_t *aPtr);

dds3Basic_t *dds3AddMatterObjectBasic(s32 serialID, f32 *pos, f32 *rot);

#endif
