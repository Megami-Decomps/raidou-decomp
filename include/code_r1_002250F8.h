#ifndef CODE_R1_002250_F8_H
#define CODE_R1_002250_F8_H

#include "common.h"

/* Each serialized entry expands to itself and additionalCount children.
 * Animation sampling uses additionalCount as the segment length, with one
 * boundary frame between segments. payloadSize advances through the payloads. */
typedef struct EffExpandedEntry {
    s32 additionalCount;
    u32 payloadSize;
    u8 unk08[8];
} EffExpandedEntry;

typedef char EffExpandedEntrySizeCheck[(sizeof(EffExpandedEntry) == 0x10) ? 1 : -1];

/* Reference-counted texture payload placed at the end of its allocation. */
typedef struct RefObj {
    u8 *base;                         /* 0x00 retained allocation base */
    u8 *pixels;                       /* 0x04 image bytes */
    u8 *palette;                      /* 0x08 palette bytes */
    s32 paletteWidth;                 /* 0x0C */
    s32 paletteHeight;                /* 0x10 */
    s32 refCount;                     /* 0x14 retained references to this texture */
    s32 index;                        /* 0x18 caller-selected texture index */
    struct sdfMemHandle_t *allocationHandle; /* 0x1C final-release descriptor */
} RefObj;

/* Kind-7 wrapper owns one allocation and retains a RefObj per entry.
 * totalEntryCount is also the animation's total track length. */
typedef struct EffExpandedList {
    u8 unk00[4];
    u32 count;
    u32 flags;
    u8 unk0C[4];
    EffExpandedEntry *entries;
    struct RefObj **handles;
    s32 totalEntryCount;
    u32 refCount;
    s32 unk20;
    struct sdfMemHandle_t *allocation;
} EffExpandedList;

typedef char EffExpandedListSizeCheck[(sizeof(EffExpandedList) == 0x28) ? 1 : -1];

void effCacheTexDelete(EffExpandedList *holder);

typedef char RefObjSizeCheck[sizeof(RefObj) == 0x20 ? 1 : -1];

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

/* Serialized texture-resource header; payload begins after this 0x40-byte prefix. */
typedef struct SdfTextureFileHeader {
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
} SdfTextureFileHeader;

typedef char SdfTextureFileHeader_size_must_be_0x40[
    (sizeof(SdfTextureFileHeader) == 0x40) ? 1 : -1];

EffExpandedList *Copy_effCacheTexList(EffExpandedList *obj);

RefObj *effCreateSharedTextureReference(struct SdfTextureFileHeader *source);

RefObj *effCloneSharedReferenceWithValue(struct SdfTextureFileHeader *source, u32 textureIndex);

void effReleaseSharedReference(RefObj *obj);

/* Retain returns the same object after incrementing its object and shared-texture counts. */
RefObj *effRetainSharedReference(RefObj *obj);

struct effPartsHandle_t;
struct opcHandle_t;

struct opcHandle_t *Copy_effModel(struct effPartsHandle_t *handle);
void Draw_effHelper(struct effPartsHandle_t *handle);
void ResourceCopy_effModel(struct effPartsHandle_t *dstHandle, struct effPartsHandle_t *srcHandle);

#endif
