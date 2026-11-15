#ifndef DDS3LIGHTOBJECTBASIC_H
#define DDS3LIGHTOBJECTBASIC_H

#include "common.h"
#include "basic.h"

dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *basic);
void dds3LinkLightObjectToUnitObject(dds3Basic_t *lightbasic, dds3Basic_t *unitbasic, s32 frame);
void dds3SetLightObjectPathResoruce(dds3Basic_t *basic, fldResourceLightPath_t *resource);
void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *unitbasic, s32 frame);

#endif
