/* Declarations of smt3-decomp src/smt3/sdf/sdfDelayedResrc.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFDELAYEDRESRC_H
#define SHARED_SDF_SDFDELAYEDRESRC_H

#include "common.h"

#include "sdf_types.h"

/* Declared here because no header carries these. */
extern s32 SignalSema(s32 aSemaId);

extern s32 WaitSema(s32 sema);

#define SDF_PENDING_LAST_SLOT 1

struct sdfDelayedResrcCB_t;

struct sdfDelayedResrcItems_t;

typedef struct sdfDelayedResrcChunk_t sdfDelayedResrcChunk_t;

/* Deferred handlers share one address-word argument at the native dispatch
 * boundary, while their C consumers interpret that word as different owners. */
typedef void (*SdfPendingCallback)(void *);

void sdfChipFreeDirect(void *aPtr);

#define SDF_PENDING_BUFFER_CAPACITY 0x3F

#define SDF_PENDING_SLOT_COUNT 2

extern s8 sdfDelayedResrcFlag;

/* Interrupt-synchronized deferred callback owner (0x08). */
typedef struct sdfDelayedResrcCB_t {
    SdfPendingCallback freeFuncPtr;
    sdfDelayedResrcChunk_t *chunk;
} sdfDelayedResrcCB_t;

/* A queued batch links its next chunk before 63 deferred owner addresses. */
typedef struct sdfDelayedResrcItems_t {
    struct sdfDelayedResrcItems_t *next;
    void *items[SDF_PENDING_BUFFER_CAPACITY];
} sdfDelayedResrcItems_t;

extern sdfDelayedResrcChunk_t *sdfDelayedResrcLink;

void *sdfChipAllocDirect(s32 aSize);

extern s32 EIntr(void);

extern saddr_t D_005520A8[2];

extern s32 DIntr(void);

extern s32 sdfDelayedResrcSemaId;

extern s32 sdfCreateSema(u32, u32, u32);

typedef struct effDebugPolyParams_t {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} effDebugPolyParams_t;

struct sdfDelayedResrcChunk_t {
    sdfDelayedResrcChunk_t *next;
    sdfDelayedResrcCB_t *owner;
    sdfDelayedResrcItems_t *items;
    s32 itemsFreeCount;
};

void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, SdfPendingCallback aFreeFunc);

void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

sdfDelayedResrcChunk_t *sdfDelayedResrcClose(void);

void sdfDelayedResrcRelease(sdfDelayedResrcChunk_t *aChunk);

u64 sdfDelayedResrcCheckFunc(void);

u64 sdfDelayedResrcCheck(void);

void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, SdfPendingCallback aFreeFunc);

void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

sdfDelayedResrcChunk_t *sdfDelayedResrcClose(void);

void sdfDelayedResrcRelease(sdfDelayedResrcChunk_t *aChunk);

void sdfDelayedResrcProc(void);

u64 sdfDelayedResrcCheckFunc(void);

u64 sdfDelayedResrcCheck(void);

#endif
