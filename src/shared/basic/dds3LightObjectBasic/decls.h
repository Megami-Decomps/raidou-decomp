/* Declarations of smt3-decomp src/smt3/basic/dds3LightObjectBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3LIGHTOBJECTBASIC_H
#define SHARED_BASIC_DDS3LIGHTOBJECTBASIC_H

#include "common.h"

#include "basic.h"

#include "ps2asm.h"

#include "sdfFrame.h"

typedef struct
{
    sdfVector4_t *sources[4];
    sdfVector4_t ambient;
} dds3SceneLightParam_t;

extern dds3SceneLightParam_t dds3LightParam;

SYMBOL_AT(sdfVector4_t, dds3LightObjectAmbient, dds3LightParam, 0x10);

extern void evtLinkLightObjectBasic(void *h, dds3Basic_t *light, s32 frame);

extern void evtUnlinkLightObjectBasic(void *h, s32 frame);

struct sdf3DLightParallelPacket_t;

typedef struct dds3LightObjectBasicWorkData_t
{
    sdf3DLight_t Light;
    sdfVector4_t Ambient;
    f32 InsideR;
    f32 OutsideR;
    f32 Soften;
    f32 Bias;
    u32 AnimeType;
    u32 Flag;
    u32 BaseLightRGB;
    u32 BaseAmbientRGB;
    u16 SwitchFrame;
    u16 Reserve1;
    dds3ObjectBaseControlWorkData_t *ObjectBaseControlWorkData;
    struct sdf3DLightParallelPacket_t *LightPacket;
} dds3LightObjectBasicWorkData_t;

typedef struct dds3LightColorWork_t
{
    u32 word;
    u8 pad04[12];
} dds3LightColorWork_t;

typedef struct fldResourceLight_t
{
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

extern void *memset(void *a, s32 b, size_t c);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern s32 dds3InitPoseSet(dds3Basic_t *basic);

extern void dds3FreePoseSet(dds3Basic_t *basic);

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 flag);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);

s32 lightconstructor(dds3Basic_t *basic);

void lightdestructor(dds3Basic_t *basic);

/* vu0 routine: blend the stored light and ambient colours toward the target */
s32 lightmainprocedure(dds3Basic_t *basic);

s32 lightdrawprocedure(dds3Basic_t *basic);

dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *basic);

dds3Basic_t *dds3AddLightObjectBasic(u32 serial);

void dds3DeleteLightObjectBasic(dds3Basic_t *basic);

/* vu0 routine: pack the live scene light and ambient colours */
void dds3LinkSceneLight(dds3Basic_t *basic, s32 frame);

void dds3UnlinkSceneLight(dds3Basic_t *basic);

void dds3LinkLightObjectToUnitObject(dds3Basic_t *lightbasic, dds3Basic_t *unitbasic, s32 frame);

void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *unitbasic, s32 frame);

void dds3SetLightObjectResoruce(dds3Basic_t *basic, fldResourceLight_t *resource);

void dds3SetLightObjectPathResoruce(dds3Basic_t *basic, fldResourceLightPath_t *resource);

s32 dds3AllocEventBasicWorkData(dds3Basic_t *basic);

#endif
