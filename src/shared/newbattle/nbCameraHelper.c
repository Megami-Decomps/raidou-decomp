#include "common.h"
#include "raidou_alloc.h"
#include "libsdf.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfTex.h"
#include "sdk_libkernl.h"
#include "sdk_libc.h"
#include "sdf2D.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "kernel_draw.h"
#include "sdfDevCons.h"
#include "dds3GlobalWork.h"
#include "dds3PadManager.h"

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

/* sdfGrOtTable entry the file list window draws into. */
#ifdef VERSION_RAIDOU2
#define NB_EFFECT_FILE_LIST_OT 104
#else
#define NB_EFFECT_FILE_LIST_OT 103
#endif

extern char dds3EffectResourcePathFormat[];


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
void dds3EffectFileListWindowDrawSprite(nbCameraHelper_effFLWinHandle_t *work);
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

/* body: matched in raidou @ 61f09dc: src/shared/code_r1_0017BBD8.c:466, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void nbEffectFileListCreateSub(s32 directoryHandle) {
    if (sdfHddValid == 0) {
        return;
    }
    sceDclose(directoryHandle);
}

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

/* body: matched in smt3 @ 9401f79e: src/smt3/newbattle/nbCameraHelper.c:158, near, same function 0.97 (words 270/270, calls 0.92, shape 1.00) */
nbCameraHelper_effFileListHandle_t *dds3EffectFileListCreate(char *path, s32 typeMask) {
    dds3EffectDirEnt_t entry;
    s32 directory;
    nbCameraHelper_effFileListHandle_t *root;
    dds3EffectFileNode_t *node;
    dds3EffectFileNode_t *previous;
    s32 length;
    EffResourceKind type;
    directory = dds3EffectDirOpen(path);
    root = sdfChipAllocDirect(sizeof(*root));
    if (sdfHddValid != 0) {
        root->directoryPath = sdfChipAllocDirect(strlen(path) + 1);
        strcpy(root->directoryPath, path);
    } else {
        root->directoryPath = sdfChipAllocDirect(strlen(path) + 1);
        strcpy(root->directoryPath, path);
    }
    previous = NULL;
    root->head = NULL;
    root->resourceCount = 0;
    while ((length = dds3EffectDirRead(directory, &entry)) > 0) {
        if ((entry.flags & 0xF000) == 0x1000) continue;
        if (length < 5) continue;
        if (length >= 0x2F) continue;
        if ((typeMask & 0x10 ? entry.name[length - 3] : entry.name[length - 4]) != '.') continue;
        type = EFF_RESOURCE_KIND_NONE;
        if ((typeMask & 1) &&
            ((entry.name[length-3]=='t' && entry.name[length-2]=='m' && entry.name[length-1]=='x') ||
             (entry.name[length-3]=='T' && entry.name[length-2]=='M' && entry.name[length-1]=='X'))) type = EFF_RESOURCE_KIND_TMX;
        else if ((typeMask & 2) &&
            ((entry.name[length-3]=='p' && entry.name[length-2]=='2' && entry.name[length-1]=='a') ||
             (entry.name[length-3]=='P' && entry.name[length-2]=='2' && entry.name[length-1]=='A'))) type = EFF_RESOURCE_KIND_P2A;
        else if ((typeMask & 4) &&
            ((entry.name[length-3]=='d' && entry.name[length-2]=='3' && entry.name[length-1]=='p') ||
             (entry.name[length-3]=='D' && entry.name[length-2]=='3' && entry.name[length-1]=='P'))) type = EFF_RESOURCE_KIND_D3P;
        else if ((typeMask & 8) &&
            ((entry.name[length-3]=='b' && entry.name[length-2]=='e' && entry.name[length-1]=='d') ||
             (entry.name[length-3]=='B' && entry.name[length-2]=='E' && entry.name[length-1]=='D'))) type = EFF_RESOURCE_KIND_BED;
        else if ((typeMask & 0x10) &&
            ((entry.name[length-2]=='p' && entry.name[length-1]=='b') ||
             (entry.name[length-2]=='P' && entry.name[length-1]=='B'))) type = EFF_RESOURCE_KIND_PB;
        else if ((typeMask & 0x20) &&
            ((entry.name[length-3]=='p' && entry.name[length-2]=='c' && entry.name[length-1]=='f') ||
             (entry.name[length-3]=='P' && entry.name[length-2]=='C' && entry.name[length-1]=='F'))) type = EFF_RESOURCE_KIND_PCF;
        if (type == EFF_RESOURCE_KIND_NONE) continue;
        node = sdfChipAllocDirect(sizeof(*node));
        node->type = type;
        strcpy(node->name, entry.name);
        if (previous != NULL) previous->next = node;
        else root->head = node;
        node->next = NULL;
        node->previous = previous;
        root->resourceCount++;
        previous = node;
    }
    nbEffectFileListCreateSub(directory);
    return root;
}

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

nbCameraHelper_effFLWinHandle_t *dds3EffectFileListWindowCreate(nbCameraHelper_effFileListHandle_t *a) {
    nbCameraHelper_effFLWinHandle_t *resource = sdfChipAllocDirect(sizeof(*resource));

    resource->x = 0;
    resource->y = 0xC8;
    resource->result = 0;
    resource->resourceCount = a->resourceCount;
    resource->unk10 = 0;
    resource->selectedIndex = 0;
    resource->visibleIndex = 0;
    resource->previewActive = 0;
    resource->repeatDelay = 0;
    resource->drawSurface = NB_EFFECT_FILE_LIST_OT;
    resource->borderColor = 0x40806020;
    resource->fillColor = 0x30000000;
    resource->firstVisibleEntry = a->head;
    resource->selectedEntry = a->head;
    resource->cachedEntry = 0;
    resource->textureHandle = 0;
    resource->resourceList = a;
    return resource;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/newbattle/nbCameraHelper.c:254, rework, same function 1.00 (words 288/288, calls 1.00, shape 1.00) */
s32 dds3EffectFileListWindowDraw(nbCameraHelper_effFLWinHandle_t *a) {
    dds3EffectFileNode_t *visibleEntry;
    sdfDrawTag_t *packetList;
    sdfGrOt_t *surface;
    s32 drawnRows;
    s32 contentRow;

    if (a->repeatDelay != 0) {
        a->repeatDelay--;
    } else if (a->firstVisibleEntry == NULL) {
        if (dds3PadBitTable[1] < 0) {
            a->result = 2;
        }
    } else if (a->result == 0) {
        dds3EffectFileNode_t *navigationEntry;
        dds3EffectFileNode_t *selectedEntry;
        s32 navigationStep;
        if ((dds3PadBitTable[6] & 2) != 0) {
            if (a->selectedEntry->previous != NULL) {
                a->selectedEntry = a->selectedEntry->previous;
                a->selectedIndex--;
                if (a->visibleIndex != 0) {
                    a->visibleIndex--;
                }
                if (a->selectedEntry == a->firstVisibleEntry) {
                    if (a->selectedEntry->previous != NULL) {
                        a->firstVisibleEntry = a->selectedEntry->previous;
                        a->visibleIndex = 1;
                    }
                }
            }
        } else if ((dds3PadBitTable[7] & 2) != 0) {
            if (a->selectedEntry->next != NULL) {
                a->selectedEntry = a->selectedEntry->next;
                a->selectedIndex++;
                a->visibleIndex++;
                if (a->visibleIndex == 0xE) {
                    if (a->firstVisibleEntry->next != NULL) {
                        a->firstVisibleEntry = a->firstVisibleEntry->next;
                        a->visibleIndex = 0xD;
                    }
                }
            }
        } else if ((dds3PadBitTable[9] & 2) != 0) {
            selectedEntry = a->selectedEntry;
            for (navigationStep = 0; navigationStep < 0xF; navigationStep++) {
                navigationEntry = selectedEntry->previous;
                if (navigationEntry != NULL) {
                    a->selectedEntry = navigationEntry;
                    a->selectedIndex--;
                    if (a->visibleIndex != 0) {
                        a->visibleIndex--;
                    }
                    selectedEntry = a->selectedEntry;
                    if (selectedEntry == a->firstVisibleEntry) {
                        dds3EffectFileNode_t *previous = selectedEntry->previous;
                        if (previous != NULL) {
                            a->firstVisibleEntry = previous;
                            a->visibleIndex = 1;
                        }
                    }
                }
            }
        } else if ((dds3PadBitTable[11] & 2) != 0) {
            for (navigationStep = 0; navigationStep < 0xF; navigationStep++) {
                navigationEntry = a->selectedEntry->next;
                if (navigationEntry != NULL) {
                    a->selectedEntry = navigationEntry;
                    a->selectedIndex++;
                    a->visibleIndex++;
                    if (a->visibleIndex == 0xE) {
                        dds3EffectFileNode_t *next = a->firstVisibleEntry->next;
                        if (next != NULL) {
                            a->firstVisibleEntry = next;
                            a->visibleIndex = 0xD;
                        }
                    }
                }
            }
        } else if (dds3PadBitTable[1] < 0) {
            a->result = 2;
        } else if (dds3PadBitTable[3] < 0) {
            a->result = 1;
        } else {
            dds3EffectFileNode_t *cachedEntry = a->cachedEntry;
            selectedEntry = a->selectedEntry;
            if (cachedEntry != selectedEntry) {
                s32 category = selectedEntry->type;
                switch (category) {
                case 1: {
                    char resourceName[0x70];
                    sprintf(resourceName, dds3EffectResourcePathFormat,
                        a->resourceList->directoryPath, selectedEntry->name);
                    dds3EffectListSetTexture(a, resourceName);
                    a->previewActive = category;
                    selectedEntry = a->selectedEntry;
                    break;
                }
                case 2:
                    a->previewActive = 0;
                    break;
                case 4:
                    a->previewActive = 0;
                    break;
                default:
                    a->previewActive = 0;
                    break;
                }
                a->cachedEntry = selectedEntry;
            }
            if (a->previewActive != 0) {
                dds3EffectFileListWindowDrawSprite(a);
            }
        }
    }

    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, EE_ADDR(dds3PrimFrameRect(
        0x8000 - (a->x << 4), 0x8000 - (a->y << 3),
        0xFEFFFF, 0xC00, 0x620, a->fillColor, a->borderColor)));
    if (a->previewActive != 0) {
        sdfDrawTagLink(packetList, EE_ADDR(dds3PrimFrameRect(
            0x8000 - (a->x << 4),
            0x8000 - ((a->y - 0xC4) << 3),
            0xFEFFFF, 0x800, 0x400, 0, 0x40806020)));
    }
    contentRow = a->y - 2;
    sdfDrawTagLink(packetList, EE_ADDR(sdfDevPrintf(
        0x8000 - ((a->x - 2) << 4),
        0x8000 - (contentRow << 3), 0xFF0000, 5,
        a->resourceList->directoryPath)));
    contentRow -= 0xC;
    visibleEntry = a->firstVisibleEntry;
    drawnRows = 0;
    while (visibleEntry != NULL) {
        s32 selectionFlags = visibleEntry == a->selectedEntry ? 4 : 0;
        sdfDrawTagLink(packetList, EE_ADDR(sdfDevPrintf(
            0x8000 - ((a->x - 2) << 4),
            0x8000 - (contentRow << 3), 0xFF0000,
            selectionFlags, visibleEntry->name)));
        drawnRows++;
        contentRow -= 0xC;
        if (drawnRows >= 0xF) {
            break;
        }
        visibleEntry = visibleEntry->next;
    }
    surface = &sdfGrOtTable[a->drawSurface];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, packetList);
    return a->result;
}

void dds3EffectFileListWindowDelete(nbCameraHelper_effFLWinHandle_t *a) {
    sdfTexHandle_t *texture = a->textureHandle;

    if (texture != NULL) {
        sdfTexFreeDelayed(texture);
        a->textureHandle = NULL;
    }
    sdfChipFreeDirect(a);
}

void dds3EffectFileListWindowSetPos(nbCameraHelper_effFLWinHandle_t *a, s32 b, s32 c) {
    a->x = b;
    a->y = c;
}

u32 nbLeaf58(nbCameraHelper_effFLWinHandle_t *work) {
    return work->selectedIndex;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/newbattle/nbCameraHelper.c:426, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u32 dds3EffectFileListWindowGetState(nbCameraHelper_effFLWinHandle_t *a) {
    return a->result;
}

u32 dds3EffectFileListWindowGetFilename(nbCameraHelper_effFLWinHandle_t *a, char *b) {
    sprintf(b, dds3EffectResourcePathFormat, a->resourceList->directoryPath, a->selectedEntry->name);
    return a->selectedEntry->type;
}

void dds3EffectFileListWindowSetOt(nbCameraHelper_effFLWinHandle_t *hEffFLWin, u32 ot) {
    hEffFLWin->repeatDelay = ot;
}

void dds3EffectFileListWindowSetPadWait(nbCameraHelper_effFLWinHandle_t *hEffFLWin, u32 wait) {
    hEffFLWin->drawSurface = wait;
}

void dds3EffectFileListWindowSetWindowColor(nbCameraHelper_effFLWinHandle_t *hEffFLWin, u32 frame_c, u32 bg_c) {
    hEffFLWin->borderColor = frame_c;
    hEffFLWin->fillColor = bg_c;
}

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

void dds3EffectFileListWindowDrawSprite(nbCameraHelper_effFLWinHandle_t *work)
{
    sdfDrawTag_t *list;
    void *sprite;
    dds3EffectSpriteVertex_t *vertex;
    s32 left;
    s32 top;

    if (work->textureHandle == NULL) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sprite = sdf2DAllocSprite(1);
    vertex = (dds3EffectSpriteVertex_t *)sdf2DGetPrimPtr(sprite);
    vertex->a = 0x80;
    vertex->b = 0x80;
    vertex->g = 0x80;
    vertex->r = 0x80;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[1].u = work->textureHandle->width << 4;
    vertex->corner[1].v = work->textureHandle->height << 4;
    left = 0x8000 - (work->x << 4);
    vertex->corner[0].x = left;
    left += 0x800;
    top = 0x8000 - ((work->y - 0xC4) << 3);
    vertex->corner[0].y = top;
    vertex->corner[0].mask = 0xFF0000;
    vertex->corner[0].flag = 0;
    vertex->corner[1].x = left;
    vertex->corner[1].y = top + 0x400;
    vertex->corner[1].mask = 0xFF0000;
    vertex->corner[1].flag = 0;
    sdf2DLinkTexPacket(list, work->textureHandle, 0);
    sdfDrawTagLink(list, EE_ADDR(sprite));
    {
        sdfGrOt_t *ot = &sdfGrOtTable[NB_EFFECT_FILE_LIST_OT];

        ot->regFunc(ot, list);
    }
}

char dds3EffectDirPathFormat[8] = "pfs0:%s";

char dds3EffectResourcePathFormat[5] = "%s%s";
