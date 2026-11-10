#ifndef DDS_EFF_RIBBON_WORK_H

#define DDS_EFF_RIBBON_WORK_H
#include "common.h"

#include "eff_ref_obj.h"

#include "sdf.h"

/* Shared 0x34-byte backing record used by the alternate ribbon renderer.
 * surfaceIndex comes from the alpha track; drawMode is the caller's byte
 * mode, which the renderer tests only for zero/nonzero. */
typedef struct EffRibbonWork {
    u32 count;                 /* 0x00 */
    u32 surfaceIndex;          /* 0x04 */
    u32 color;                 /* 0x08 */
    s32 rowStride;             /* 0x0C */
    s32 repeat;                /* 0x10 */
    u8 drawMode;               /* 0x14 */
    u8 pad_15[3];
    RefObj *resource;          /* 0x18: null selects the shared wind texture */
    u32 *colors;               /* 0x1C */
    u8 *positions;             /* 0x20 */
    u8 *uvs;                    /* 0x24 */
    u8 *extra;                  /* 0x28 */
    sdfMat_t *handle;           /* 0x2C */
    struct sdfMemHandle_t *allocation; /* 0x30 */
} EffRibbonWork;

EffRibbonWork *effCreateRibbonWork(u32 count, u32 repeat);

EffRibbonWork *effCreateRibbonWithSharedResource(
    u32 count, u32 repeat, struct SdfTextureFileHeader *resource);

EffRibbonWork *effCloneRibbonWithSharedResource(EffRibbonWork *source);

void effSharedAssetReferenceRelease(EffRibbonWork *work);

typedef char EffRibbonWorkSizeCheck[(sizeof(EffRibbonWork) == 0x34) ? 1 : -1];

#endif
