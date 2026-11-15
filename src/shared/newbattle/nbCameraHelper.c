#include "common.h"
#include "raidou_alloc.h"
#include "libsdf.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfTex.h"
#include "sdk_libkernl.h"

/* Owned file list: names stay inline in each 0x3C-byte node. */
typedef struct dds3EffectFileNode_t
{
    s32 type;
    char name[0x30];
    struct dds3EffectFileNode_t *previous;
    struct dds3EffectFileNode_t *next;
} dds3EffectFileNode_t;

typedef struct
{
    s32 resourceCount;
    char *directoryPath;
    dds3EffectFileNode_t *head;
} nbCameraHelper_effFileListHandle_t;

/* Complete 0x44-byte browser owner, including its retained preview texture. */
typedef struct
{
    s32 x;
    s32 y;
    s32 result;
    s32 resourceCount;
    u32 unk10;
    s32 selectedIndex;
    s32 visibleIndex;
    u32 previewActive;
    u32 repeatDelay;
    u32 drawSurface;
    u32 borderColor;
    u32 fillColor;
    dds3EffectFileNode_t *firstVisibleEntry;
    dds3EffectFileNode_t *selectedEntry;
    dds3EffectFileNode_t *cachedEntry;
    sdfTexHandle_t *textureHandle;
    nbCameraHelper_effFileListHandle_t *resourceList;
} nbCameraHelper_effFLWinHandle_t;

extern char dds3EffectResourcePathFormat[];

#include "sdk_libc.h"

extern u8 sdfHddValid;
extern s32 dds3EffectDirOpen(void *name);

/* One textureless sprite as sdf2D lays it out: the colour, then two corners
 * of 0x20 bytes each. */
typedef struct
{
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} dds3EffectSpriteCorner_t;

typedef struct
{
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    dds3EffectSpriteCorner_t corner[2];
} dds3EffectSpriteVertex_t;

void dds3EffectListSetTexture(nbCameraHelper_effFLWinHandle_t *work, char *resource);

/* SDK directory entry filled by the directory-read provider. */
typedef struct
{
    u32 flags;
    u8 opaque04[0x3C];
    char name[0x100];
    void *privateData;
} dds3EffectDirEnt_t;

typedef enum EffResourceKind {
    EFF_RESOURCE_KIND_NONE = 0, EFF_RESOURCE_KIND_TMX = 1, EFF_RESOURCE_KIND_P2A = 2,
    EFF_RESOURCE_KIND_D3P = 4, EFF_RESOURCE_KIND_BED = 8, EFF_RESOURCE_KIND_PB = 0x10,
    EFF_RESOURCE_KIND_PCF = 0x20
} EffResourceKind;

extern s32 dds3EffectDirRead(s32 directory, dds3EffectDirEnt_t *entry);

#define EFF_BUILTIN_NAME_COUNT 0x2F

#define EFF_DIRECTORY_FLAG_CLEAR 0x1000

#define EFF_DIRECTORY_PATH_BYTES 0x70

extern char *dds3EffectFileNameTable[];
extern char dds3EffectDirPathFormat[];
extern u32 dds3EffectBuiltinNameIndex;
s32 dds3EffectDirOpen(void *name);
s32 dds3EffectDirRead(s32 directory, dds3EffectDirEnt_t *entry);
void dds3EffectFileListDelete(nbCameraHelper_effFileListHandle_t *a);
void dds3EffectFileListWindowDelete(nbCameraHelper_effFLWinHandle_t *a);
u32 dds3EffectFileListWindowGetFilename(nbCameraHelper_effFLWinHandle_t *a, char *b);
void dds3EffectListSetTexture(nbCameraHelper_effFLWinHandle_t *work, char *textureResource);
extern char dds3EffectDirPathFormat[8];
extern char dds3EffectResourcePathFormat[5];

/* Debug filesystem mode opens the formatted pfs0 directory; otherwise reset built-in iteration.
   Return the native directory-open result, or zero for built-in mode. */
s32 dds3EffectDirOpen(void *name) {
    u8 path[EFF_DIRECTORY_PATH_BYTES];

    if (sdfHddValid != 0) {
        sprintf(path, dds3EffectDirPathFormat, name);
        return sceDopen(path);
    } else {
        dds3EffectBuiltinNameIndex = 0;
        return 0;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", nbEffectFileListCreateSub);

/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 dds3EffectDirRead(s32 directory, dds3EffectDirEnt_t *entry) {
    if (sdfHddValid != 0) {
        return sceDread(directory, entry);
    }
    if ((u32)dds3EffectBuiltinNameIndex >= EFF_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(entry->name, dds3EffectFileNameTable[dds3EffectBuiltinNameIndex]);
    entry->flags &= ~EFF_DIRECTORY_FLAG_CLEAR;
    dds3EffectBuiltinNameIndex++;
    return strlen(entry->name);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", dds3EffectFileListCreate);

void dds3EffectFileListDelete(nbCameraHelper_effFileListHandle_t *a) {
    dds3EffectFileNode_t *node = a->head;

    if (node != NULL) {
        do {
            dds3EffectFileNode_t *next = node->next;
            sdfChipFreeDirect(node);
            node = next;
        } while (node != NULL);
    }
    sdfChipFreeDirect(a->directoryPath);
    sdfChipFreeDirect(a);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", dds3EffectFileListWindowCreate);

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", dds3EffectFileListWindowDraw);

void dds3EffectFileListWindowDelete(nbCameraHelper_effFLWinHandle_t *a) {
    sdfTexHandle_t *texture = a->textureHandle;

    if (texture != NULL) {
        sdfTexFreeDelayed(texture);
        a->textureHandle = NULL;
    }
    sdfChipFreeDirect(a);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", nbLeaf57);

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", nbLeaf58);

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", dds3EffectFileListWindowGetState);

u32 dds3EffectFileListWindowGetFilename(nbCameraHelper_effFLWinHandle_t *a, char *b) {
    sprintf(b, dds3EffectResourcePathFormat, a->resourceList->directoryPath, a->selectedEntry->name);
    return a->selectedEntry->type;
}

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", nbLeaf59);

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", nbLeaf60);

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", nbLeaf61);

void dds3EffectListSetTexture(nbCameraHelper_effFLWinHandle_t *work, char *textureResource) {
    sdfTexHandle_t *texture;
    sdfMemHandle_t *loadedResource;
    u32 resourceWords[4];

    if (work->textureHandle != NULL) {
        sdfTexFreeDelayed(work->textureHandle);
        work->textureHandle = NULL;
    }
    loadedResource = sdfDevLoadFileEx(textureResource, resourceWords, 0);
    texture = sdfTexSet(EE_ADDR_AS(void *, (resourceWords[0])));
    work->textureHandle = texture;
    sdfMemFreeDirect(loadedResource);
}

INCLUDE_ASM(const s32, "newbattle/nbCameraHelper", dds3EffectFileListWindowDrawSprite);

char dds3EffectDirPathFormat[8] = "pfs0:%s";

char dds3EffectResourcePathFormat[5] = "%s%s";
