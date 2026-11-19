#ifndef CODE_R1_002250_F8_H
#define CODE_R1_002250_F8_H

#include "common.h"
#include "sdf_resource.h"

/* Each serialized entry expands to itself and additionalCount children.
 * Animation sampling uses additionalCount as the segment length, with one
 * boundary frame between segments. payloadSize advances through the payloads. */
typedef struct effExpandedEntry_t {
    s32 additionalCount;
    u32 payloadSize;
    u8 unk08[8];
} effExpandedEntry_t;

typedef char effExpandedEntrySizeCheck[(sizeof(effExpandedEntry_t) == 0x10) ? 1 : -1];

/* Reference-counted texture payload placed at the end of its allocation. */
typedef struct effRefObj_t {
    u8 *base;                         /* 0x00 retained allocation base */
    u8 *pixels;                       /* 0x04 image bytes */
    u8 *palette;                      /* 0x08 palette bytes */
    s32 paletteWidth;                 /* 0x0C */
    s32 paletteHeight;                /* 0x10 */
    s32 refCount;                     /* 0x14 retained references to this texture */
    s32 index;                        /* 0x18 caller-selected texture index */
    struct sdfMemHandle_t *allocationHandle; /* 0x1C final-release descriptor */
} effRefObj_t;

/* Kind-7 wrapper owns one allocation and retains a effRefObj_t per entry.
 * totalEntryCount is also the animation's total track length. */
typedef struct ctexlHandle_t {
    u8 unk00[4];
    u32 count;
    u32 flags;
    u8 unk0C[4];
    effExpandedEntry_t *entries;
    struct effRefObj_t **handles;
    s32 totalEntryCount;
    u32 refCount;
    s32 unk20;
    struct sdfMemHandle_t *allocation;
} ctexlHandle_t;

typedef char ctexlHandleSizeCheck[(sizeof(ctexlHandle_t) == 0x28) ? 1 : -1];

typedef struct effCacheTexList_Anim_t {
    f32 scaleX;     // 0x00
    f32 scaleY;     // 0x04
    f32 angle;      // 0x08
    s32 segment;    // 0x0C
} effCacheTexList_Anim_t;

void effCacheTexAnimGet(const ctexlHandle_t *set, u32 frame, effCacheTexList_Anim_t *out);
sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *surface, effRefObj_t *source);
sdfTexHandle_t *effAssignSampledSegmentReference(const ctexlHandle_t *set, sdfGrOt_t *target,
                                                 const effCacheTexList_Anim_t *sample);

void effCacheTexDelete(ctexlHandle_t *holder);

typedef char effRefObjSizeCheck[sizeof(effRefObj_t) == 0x20 ? 1 : -1];

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

/* Serialized texture-resource header; payload begins after this 0x40-byte prefix. */
typedef struct sdfTextureFileHeader_t {
    u8 unk00;
    u8 payloadOffsetFlags; /* 0x01: high nibble adds payload offset bytes; low bits stay opaque. */
    u8 pad02[2];       /* 0x02 */
    u32 byteCount;     /* 0x04 */
    u32 magic;         /* 0x08 */
    u8 pad0C[4];       /* 0x0C */
    u8 paletteCount;   /* 0x10 */
    u8 clutFormat;     /* 0x11 */
    s16 width;         /* 0x12 */
    s16 height;        /* 0x14 */
    u8 pixelFormat;    /* 0x16 */
    u8 maxMipLevel;    /* 0x17 */
    u16 lodParameters; /* 0x18 */
    u8 unk1A;          /* 0x1A */
    SdfTextureClampMode clampMode; /* 0x1B: serialized GS clamp mode */
    s32 resourceKey;   /* 0x1C */
    s32 battleTextureSlot; /* 0x20: selects one of two battle-model texture headers */
    u8 pad24[0x1C];    /* 0x24 */
} sdfTextureFileHeader_t;

typedef char sdfTextureFileHeader_size_must_be_0x40[
    (sizeof(sdfTextureFileHeader_t) == 0x40) ? 1 : -1];

ctexlHandle_t *Copy_effCacheTexList(ctexlHandle_t *obj);

effRefObj_t *effCreateSharedTextureReference(struct sdfTextureFileHeader_t *source);

effRefObj_t *effCloneSharedReferenceWithValue(struct sdfTextureFileHeader_t *source, u32 textureIndex);

void effReleaseSharedReference(effRefObj_t *obj);

/* Retain returns the same object after incrementing its object and shared-texture counts. */
effRefObj_t *effRetainSharedReference(effRefObj_t *obj);

/* HD effModel_Params_t: the part's 8 parameter bytes, copied whole;
 * flg nonzero leaves the model's motion unlooped. */
typedef struct effModel_Params_t {
    f32 scale;
    u8 flg;
    u8 pad05[3];
} effModel_Params_t;

/* Effect model part (HD emdlHandle_t, 0x14 chip block); the light packet is a 0xE0 chip block. */
typedef struct emdlHandle_t {
    effModel_Params_t param;
    struct mdlObj_t *handle;
    struct sdf3DLightParallelPacket_t *pLightPacket;
    u32 index_motion;
} emdlHandle_t;

SIZE_CHECK(emdlHandleSizeCheck, emdlHandle_t, 0x14);

struct effPartsHandle_t;
struct opcHandle_t;

struct fileJobPayload_t;
emdlHandle_t *Create_effModel(struct fileJobPayload_t *job);
struct opcHandle_t *Copy_effModel(struct effPartsHandle_t *handle);
void Draw_effHelper(struct effPartsHandle_t *handle);
void ResourceCopy_effModel(struct effPartsHandle_t *dstHandle, struct effPartsHandle_t *srcHandle);

#endif
