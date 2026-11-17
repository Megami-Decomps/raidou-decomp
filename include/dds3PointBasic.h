#ifndef DDS3POINTBASIC_H
#define DDS3POINTBASIC_H

#include "common.h"
#include "basic.h"
#include "field.h"

dds3Basic_t *dds3AddPointBasic(u32 serialID, fldResourcePoint_t *pData, const char *pName);
void *dds3GetWorldNodeData(dds3Basic_t *node);

#endif
