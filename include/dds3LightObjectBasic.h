#ifndef DDS3LIGHTOBJECTBASIC_H
#define DDS3LIGHTOBJECTBASIC_H

#include "common.h"
#include "basic.h"

typedef struct fldResourceLight_t {
    u32 resv;
    u32 Flag;
    u32 AnimeType;
    f32 InsideR;
    f32 OutsideR;
    f32 Soften;
    f32 Bias;
    f32 DiffuseR;
    f32 DiffuseG;
    f32 DiffuseB;
    f32 AmbientR;
    f32 AmbientG;
    f32 AmbientB;
} fldResourceLight_t;

dds3Basic_t *dds3AddLightObjectBasic(u32 serial);

dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *basic);
void dds3LinkLightObjectToUnitObject(dds3Basic_t *lightbasic, dds3Basic_t *unitbasic, s32 frame);
void dds3SetLightObjectResoruce(dds3Basic_t *basic, fldResourceLight_t *resource);
void dds3SetLightObjectPathResoruce(dds3Basic_t *basic, fldResourceLightPath_t *resource);
void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *unitbasic, s32 frame);

#endif
