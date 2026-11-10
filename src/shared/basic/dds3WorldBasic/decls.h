/* Declarations of smt3-decomp src/smt3/basic/dds3WorldBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3WORLDBASIC_H
#define SHARED_BASIC_DDS3WORLDBASIC_H

#include "common.h"

#include "basic.h"

#include "libsdf.h"

extern dds3Basic_t *dds3TheWorld;

void *sdfChipAllocDirect(u32 aSize);

void *sdfMemAllocDirect(u32 aSize);

void *sdfMemLock(sdfMemHandle_t *aHandle);

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

s32 dds3CallBasicMainProc(dds3Basic_t *pBasic);

s32 dds3CallBasicDrawProc(dds3Basic_t *basic);

void dds3DeleteScene(dds3Basic_t *scene);

void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);

u32 dds3SetBasicArray(dds3BasicArrayHandle_t *hndl, dds3Basic_t *data);

void sdfChipFreeDirect(void *aPtr);

dds3Basic_t *dds3AllocBasic(u32 type);

void dds3DeleteBasic(dds3Basic_t *basic);

u32 dds3AddBasicArray(dds3BasicArrayHandle_t *hndl, s32 bufcnt);

u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *hndl);

dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *hndl);

u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl);

u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl);

void dds3DeleteBasicArray(dds3BasicArrayHandle_t *hndl);

void dds3DeleteAllBasicArray(dds3BasicArrayHandle_t *hndl);

s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *hArray, const dds3Basic_t *pBasic);

s32 worldconstructor(dds3Basic_t *basic);

void worlddestructor(dds3Basic_t *basic);

s32 worldmainprocuder(dds3Basic_t *basic);

s32 worlddrawprocuder(dds3Basic_t *basic);

dds3Basic_t *dds3AddWorld(s32 bufcnt);

void dds3DeleteWorld(void);

void dds3SetActiveScene(dds3Basic_t *scene);

dds3Basic_t *dds3GetActiveScene(void);

void dds3SetControlScene(dds3Basic_t *scene);

dds3Basic_t *dds3GetControlScene(void);

dds3Basic_t *dds3AddScene(void);

void dds3DeleteScene(dds3Basic_t *scene);

dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 bufcnt);

void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);

/* The cursor advances to the slot after the one deleted. */
void dds3DeleteBasicArray(dds3BasicArrayHandle_t *hndl);

void dds3DeleteAllBasicArray(dds3BasicArrayHandle_t *hndl);

s32 dds3SearchDeleteBasicArray(dds3BasicArrayHandle_t *hArray, dds3Basic_t *pBasic, s32 mode);

/* The walk stops at the first empty slot, so a NULL basic never matches. */
s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *hArray, const dds3Basic_t *pBasic);

u32 dds3AddBasicArray(dds3BasicArrayHandle_t *hndl, s32 bufcnt);

u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *hndl);

u32 dds3SetBasicArray(dds3BasicArrayHandle_t *hndl, dds3Basic_t *data);

dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *hndl);

u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl);

u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl);

s32 dds3ForEachBasicArray(dds3BasicArrayHandle_t *handle, s32 (*func)(dds3Basic_t *));

#endif
