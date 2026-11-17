#ifndef DDS3EVENTBASIC_H
#define DDS3EVENTBASIC_H

#include "common.h"
#include "basic.h"

dds3Basic_t *dds3AddEventBasic(u32 serialID, void *pData, const char *pName);
void *dds3EventBasicGetResource(dds3Basic_t *pBasic);

#endif
