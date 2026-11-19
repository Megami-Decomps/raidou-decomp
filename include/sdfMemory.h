#ifndef SDFMEMORY_H
#define SDFMEMORY_H

#include "common.h"
#include "libsdf.h"
#include "sdf_resource.h"

s32 SdfIopFileRpc(const char *aName, s32 aDataSize, const void *aData, s32 *aOutSize);
sdfMemHandle_t *sdfMemAlloc2Direct(s32 aSize);
sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
#ifdef VERSION_RAIDOU2
void *sdfCallMemAllocDirect(s32 aSize);
#endif
sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 aSize);
void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);
void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
void sdfMemFreeHandleDirect(sdfMemHandle_t **aHandlePtr);
void sdfMemFreePtrDirect(void *aPtr);
sdfMemHandle_t *sdfMemGetHandleByPtr(void *aPtr);
void sdfMemGetInfo(sdfMemInfo_t *aInfo);
sdfMemHandle_t *sdfMemGetNext(sdfMemHandle_t *aHandle);
void *sdfMemGetPtr(sdfMemHandle_t *aHandle);
s32 sdfMemGetSize(const sdfMemHandle_t *aHandle);
sdfMemHandle_t *sdfMemGetTop(void);
u16 sdfMemGetType(sdfMemHandle_t *aHandle);
void sdfMemInit(u32 aSize);
void *sdfMemLock(sdfMemHandle_t *aHandle);
void sdfMemReallocRecord(sdfMemHandle_t *aHandle, s32 aSize);
void sdfMemUnlock(sdfMemHandle_t *aHandle);

#endif
