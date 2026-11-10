/* Declarations of smt3-decomp src/smt3/sdf/sdfMemory.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFMEMORY_H
#define SHARED_SDF_SDFMEMORY_H

#include "common.h"

#include "libsdf.h"

/* The SDK multi-RPC client; only its byte span matters here. */
typedef struct
{
    u8 opaque00[0x24];
} sdfIopRpcClient_t;

#define SDF_MEM_ALIGN_MASK 0x7F

#define SDF_IOP_FILE_RPC_OVERHEAD 0xC

#define SDF_IOP_FILE_RPC_REPLY_ALIGN_MASK 0x3F

extern void *malloc(size_t size);

extern s32 DIntr(void);

extern void EIntr(void);

extern void *memcpy(void *a, const void *b, size_t c);

extern u32 strlen(const char *s);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);

extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

#include "sdk_libmrpc.h"

extern sdfMemHandleCB_tag sdfMemCB;

extern sdfIopRpcClient_t sdfIopRpcClient;

extern sdfDelayedResrcCB_t sdfMemDelayedResrcCB;

/* Called when the heap cannot satisfy a request; the unit's .sdata holds it. */
extern void (*sdfMemFullCallback)(s32 size);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

void sdfMemFreeMerge(sdfMemHandle_t *aHandle);

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

sdfMemHandle_t *sdfMemGetHandleByPtr(void *aPtr);

/* Align the usable span to 128 bytes and link one free handle between the guards. */
void sdfMemInit(u32 aSize);

u16 sdfMemGetType(sdfMemHandle_t *aHandle);

sdfMemHandle_t *sdfMemGetTop(void);

/* The next handle, or NULL when that is the end sentinel. */
sdfMemHandle_t *sdfMemGetNext(sdfMemHandle_t *aHandle);

/* A handle's span runs to the next handle's start. */
s32 sdfMemGetSize(const sdfMemHandle_t *aHandle);

void *sdfMemGetPtr(sdfMemHandle_t *aHandle);

/* First-fit from the low end, splitting the handle it lands in. Never returns
 * empty-handed: when the walk reaches the end sentinel it reports the failure
 * and keeps walking. */
sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

/* First-fit from the high end: the handle keeps its tail and the remainder
 * becomes a new free handle in front of it. */
sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 aSize);

/* sdfMemAllocDirect without the full-heap callback: returns NULL instead. */
sdfMemHandle_t *sdfMemAlloc2Direct(s32 aSize);

/* Resize the span a handle owns, keeping the handle itself. Growing absorbs or
 * trims the free neighbour when it can, and otherwise moves the data and swaps
 * the two handles' contents so the caller's handle survives. */
void sdfMemReallocRecord(sdfMemHandle_t *aHandle, s32 aSize);

/* Unlink the successor, not the handle itself, and reconnect both links. */
void sdfMemFreeMerge(sdfMemHandle_t *aHandle);

/* Coalesce adjacent free handles, then recycle this handle or mark it free. */
void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

/* Resolve a data address to its handle before releasing it. */
void sdfMemFreePtrDirect(void *aPtr);

/* Clear the owner's slot before releasing the handle it held. */
void sdfMemFreeHandleDirect(sdfMemHandle_t **aHandlePtr);

/* NULL is a sentinel and is never queued. */
void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

void *sdfMemLock(sdfMemHandle_t *aHandle);

/* Never decrements zero. */
void sdfMemUnlock(sdfMemHandle_t *aHandle);

/* The address must belong to an existing used handle: the end-sentinel path
 * does not end this scan. */
sdfMemHandle_t *sdfMemGetHandleByPtr(void *aPtr);

void sdfMemGetInfo(sdfMemInfo_t *aInfo);

/* The used handle whose span contains `aAddr`; NULL at the end sentinel. */
sdfMemHandle_t *sdfMemSearchHandle(s32 ptr);

/* Pack the two lengths, the terminated name and the optional payload, then
 * return the transport error or the server's result. */
s32 SdfIopFileRpc(const char *aName, s32 aDataSize, const void *aData, s32 *aOutSize);

#endif
