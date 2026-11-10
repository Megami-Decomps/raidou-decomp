/* Declarations of smt3-decomp src/smt3/newbattle/nbCameraHelper.c, for its bodies in this directory. */
#ifndef SHARED_NEWBATTLE_NBCAMERAHELPER_H
#define SHARED_NEWBATTLE_NBCAMERAHELPER_H

#include "common.h"

#include "libsdf.h"

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

extern s8 dds3PadBitTable[];

extern char dds3EffectResourcePathFormat[];

extern sdfGrOt_t sdfGrOtTable[];

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

#include "sdk_libc.h"

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

extern sdfTexHandle_t *sdfTexSet(void *aTex);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void *sdfGrAllocPacketBuff(u32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);

extern void dds3AdminiCallSequence(s32 sequence, saddr_t data, s32 datasize, s32 restore);

extern u8 sdfHddValid;

extern u32 strlen(const char *s);

extern char *strcpy(char *dst, const char *src);

extern s32 dds3EffectDirOpen(void *name);

extern void dds3EffectDirClose(s32 directory);

extern void *sdf2DAllocSprite(s32 aNumOfSpr);

extern void *sdf2DGetPrimPtr(void *aPacketPtr);

/* The provider returns the linked packet address, even when ignored here. */
extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

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

void dds3EffectFileListWindowDrawSprite(nbCameraHelper_effFLWinHandle_t *work);

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

extern s32 sceDopen(void *path);

extern s32 sceDclose(s32 directory);

extern s32 sceDread(s32 directory, dds3EffectDirEnt_t *entry);

/* Debug filesystem mode opens the formatted pfs0 directory; otherwise reset built-in iteration.
   Return the native directory-open result, or zero for built-in mode. */
s32 dds3EffectDirOpen(void *name);

/* Run the native directory callback only in debug filesystem mode; its result is ignored. */
void dds3EffectDirClose(s32 directory);

/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 dds3EffectDirRead(s32 directory, dds3EffectDirEnt_t *entry);

/* One node per file whose extension matches a requested type bit. The 0x10
 * types use a two-character extension, so the dot sits one byte later. */
nbCameraHelper_effFileListHandle_t *dds3EffectFileListCreate(char *path, s32 typeMask);

void dds3EffectFileListDelete(nbCameraHelper_effFileListHandle_t *a);

nbCameraHelper_effFLWinHandle_t *dds3EffectFileListWindowCreate(nbCameraHelper_effFileListHandle_t *a);

s32 dds3EffectFileListWindowDraw(nbCameraHelper_effFLWinHandle_t *a);

void dds3EffectFileListWindowDelete(nbCameraHelper_effFLWinHandle_t *a);

void dds3EffectFileListWindowSetPos(nbCameraHelper_effFLWinHandle_t *a, s32 b, s32 c);

u32 dds3_1AB818(nbCameraHelper_effFLWinHandle_t *work);

u32 dds3EffectFileListWindowGetState(nbCameraHelper_effFLWinHandle_t *a);

u32 dds3EffectFileListWindowGetFilename(nbCameraHelper_effFLWinHandle_t *a, char *b);

void dds3EffectFileListWindowSetOt(nbCameraHelper_effFLWinHandle_t *hEffFLWin, u32 ot);

void dds3EffectFileListWindowSetPadWait(nbCameraHelper_effFLWinHandle_t *hEffFLWin, u32 wait);

void dds3EffectFileListWindowSetWindowColor(nbCameraHelper_effFLWinHandle_t *hEffFLWin, u32 frame_c, u32 bg_c);

void dds3EffectListSetTexture(nbCameraHelper_effFLWinHandle_t *work, char *textureResource);

void dds3EffectFileListWindowDrawSprite(nbCameraHelper_effFLWinHandle_t *work);

void InitParticleEditorProcess(void);

void InitSpriteViewerProcess(void);

#endif
