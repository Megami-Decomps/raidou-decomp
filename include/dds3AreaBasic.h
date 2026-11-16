#ifndef DDS3AREABASIC_H
#define DDS3AREABASIC_H

#include "common.h"
#include "basic.h"

dds3Basic_t *dds3AddAreaBasic(u32 serialID, char *pName);
dds3Basic_t *dds3AreaBasicGetFastArea(void);
void dds3AreaBasicRegistItem(dds3Basic_t *pAreaBasic, dds3Basic_t *pItemBasic);
void dds3AreaBasicGetItemArray(dds3Basic_t *pAreaBasic, dds3Basic_t *itemType);

#endif
