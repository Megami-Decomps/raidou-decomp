/* Declarations of smt3-decomp src/smt3/basic/dds3AreaBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3AREABASIC_H
#define SHARED_BASIC_DDS3AREABASIC_H

#include "common.h"

#include "basic.h"

typedef s32 (*dds3BasicProc_t)(dds3Basic_t *basic);

typedef struct dds3AreaBasicWorkData_t
{
    dds3BasicArrayHandle_t *items[2];
} dds3AreaBasicWorkData_t;

extern char dds3AreaBasicMsgNoList[];

extern char dds3AreaBasicMsgNoArea[];

#include "sdk_libc.h"

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type);

extern dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 bufcnt);

extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);

extern u16 dds3GetBasicArrayCount(dds3BasicArrayHandle_t *hndl);

extern void dds3ForEachBasicArray(dds3BasicArrayHandle_t *handle, dds3BasicProc_t func);

extern s32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl);

extern s32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl);

extern dds3Basic_t *dds3GetBasicArray(dds3BasicArrayHandle_t *hndl);

extern s32 dds3AddBasicArray(dds3BasicArrayHandle_t *hndl, s32 bufcnt);

extern s32 dds3SetBasicArray(dds3BasicArrayHandle_t *hndl, dds3Basic_t *data);

extern s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *hArray, dds3Basic_t *pBasic);

extern s32 dds3SearchDeleteBasicArray(dds3BasicArrayHandle_t *hArray, dds3Basic_t *pBasic, u32 mode);

extern dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);

s32 CB_ReleaseObjectBasic(dds3Basic_t *pBasic);

s32 CB_BasicDrawProcuder(dds3Basic_t *pBasic);

u32 dds3AreaBasicTypeHasItems(u32 type);

dds3BasicArrayHandle_t *dds3AreaBasicReleaseItem(dds3Basic_t *pAreaBasic, u32 pItemBasic);

s32 AreaBasicConstructor(dds3Basic_t *pBasic);

void AreaBasicDestructor(dds3Basic_t *pBasic);

s32 AreaBasicMainprocuder(dds3Basic_t *basic);

s32 AreaBasicDrawprocuder(dds3Basic_t *pBasic);

s32 CB_ReleaseObjectBasic(dds3Basic_t *pBasic);

s32 CB_BasicDrawProcuder(dds3Basic_t *pBasic);

u32 dds3AreaBasicTypeHasItems(u32 type);

dds3Basic_t *dds3AddAreaBasic(u32 serialID, char *pName);

void dds3AreaBasicRegistItem(dds3Basic_t *pAreaBasic, dds3Basic_t *pItemBasic);

/* Remove the first matching entry from the list selected by the item's type. */
void dds3AreaBasicGetItemArray(dds3Basic_t *pAreaBasic, dds3Basic_t *itemType);

dds3BasicArrayHandle_t *dds3AreaBasicReleaseItem(dds3Basic_t *pAreaBasic, u32 pItemBasic);

dds3BasicArrayHandle_t *dds3AreaBasicArrangeGetItemArray(dds3Basic_t *area, u32 itemType, dds3BasicProc_t check);

dds3Basic_t *dds3AreaBasicGetFastArea(void);

#endif
