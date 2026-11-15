#ifndef DDS3WORLDBASIC_H
#define DDS3WORLDBASIC_H

#include "common.h"
#include "basic.h"

u32 dds3AddBasicArray(dds3BasicArrayHandle_t *hndl, s32 bufcnt);
void dds3DeleteScene(dds3Basic_t *scene);
s32 dds3ForEachBasicArray(dds3BasicArrayHandle_t *handle, s32 (*func)(dds3Basic_t *));
void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);
dds3Basic_t *dds3GetActiveScene(void);
dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *hndl);
u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *hndl);
dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 bufcnt);
dds3Basic_t *dds3GetControlScene(void);
s32 dds3SearchDeleteBasicArray(dds3BasicArrayHandle_t *hArray, dds3Basic_t *pBasic, s32 mode);
u32 dds3SetBasicArray(dds3BasicArrayHandle_t *hndl, dds3Basic_t *data);
u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl);
u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl);
s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *hArray, const dds3Basic_t *pBasic);

#endif
