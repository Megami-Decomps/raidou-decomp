#include "common.h"
#include "enums.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "mdl.h"
#include "mdlManager.h"
#include "mapData.h"

#define MDL_MEM_CACHE_COUNT 14

extern sdfMemHandle_t *mdlData6[MDL_MEM_CACHE_COUNT];
extern MODELTYPE mdlData7[MDL_MEM_CACHE_COUNT];
extern s32 mdlData8[MDL_MEM_CACHE_COUNT];
extern s32 mdlCallMemGetInfoData;

/* NON_MATCHING: 18 of 20 words match.  */
#ifdef NON_MATCHING
void mapLeaf12(void) {
    s32 i;

    for (i = 0; i < MDL_MEM_CACHE_COUNT; i++) {
        mdlData6[i] = NULL;
        mdlData7[i] = 0;
        mdlData8[i] = 0;
    }
    mdlCallMemGetInfoData = 0;
}
#else
INCLUDE_ASM(const s32, "code_00143568", mapLeaf12);
#endif

INCLUDE_ASM(const s32, "code_00143568", mapCASHDATA);

/* NON_MATCHING: 55 of 80 words match.  */
#ifdef NON_MATCHING
s32 mdlCallMemGetSize(sdfMemHandle_t *handle, s32 major, s32 minor) {
    s32 i;
    s32 stored;
    s32 size;
    s32 count;
    void *source;

    stored = FALSE;
    for (i = 0; i < MDL_MEM_CACHE_COUNT; i++) {
        if (mdlData6[i] == NULL) {
            size = sdfMemGetSize(handle);
            mdlData6[i] = sdfMemAllocFromTailDirect(size);
            mdlData7[i] = major;
            mdlData8[i] = minor;
            source = sdfMemGetPtr(handle);
            memcpy(sdfMemGetPtr(mdlData6[i]), source, size);
            sdfMemFreeDirect(handle);
            stored = TRUE;
            break;
        }
    }
    count = 0;
    i = 0;
    while (i < MDL_MEM_CACHE_COUNT) {
        if (mdlData6[i++] != NULL) {
            count = i;
        }
    }
    mdlCallMemGetInfoData = count;
    return stored;
}
#else
INCLUDE_ASM(const s32, "code_00143568", mdlCallMemGetSize);
#endif

void mdlCallMemGetSize2(void) {
    s32 i;
    sdfMemHandle_t *previous;
    s32 size;
    void *source;
    sdfMemInfo_t info;

    for (i = mdlCallMemGetInfoData - 1; i >= 0; i--) {
        previous = mdlData6[i];
        if (previous != NULL) {
            size = sdfMemGetSize(previous);
            mdlData6[i] = sdfMemAllocDirect(size);
            source = sdfMemGetPtr(previous);
            memcpy(sdfMemGetPtr(mdlData6[i]), source, size);
            sdfMemFreeDirect(previous);
        }
    }
    sdfMemGetInfo(&info);
}

void mdlCallMemFreeDirect(void) {
    s32 i;

    for (i = 0; i < MDL_MEM_CACHE_COUNT; i++) {
        if (mdlData6[i] != NULL) {
            sdfMemFreeDirect(mdlData6[i]);
            mdlData6[i] = NULL;
            mdlData7[i] = 0;
            mdlData8[i] = 0;
        }
    }
    mdlCallMemGetInfoData = 0;
}

sdfMemHandle_t *mdlGetModelName(MODELTYPE aMajor, s32 aMinor) {
    s32 i;

    for (i = 0; i < mdlCallMemGetInfoData; i++) {
        if (mdlData7[i] == aMajor && mdlData8[i] == aMinor) {
            return mdlData6[i];
        }
    }
    return NULL;
}

/* NON_MATCHING: 53 of 75 words match.  */
#ifdef NON_MATCHING
void mdlCallMemGetInfo(void) {
    s32 i;
    s32 count;
    sdfMemInfo_t info;

    for (i = mdlCallMemGetInfoData - 1; i >= 0; i--) {
        if (mdlData6[i] != NULL) {
            sdfMemGetInfo(&info);
            if (info.maxFreeSize <= 0x37FFFF || info.freeSize <= 0x3FFFFF) {
                sdfMemGetPtr(mdlData6[i]);
                sdfMemFreeDirect(mdlData6[i]);
                mdlData6[i] = NULL;
                mdlData7[i] = 0;
                mdlData8[i] = 0;
            }
        }
    }
    count = 0;
    i = 0;
    while (i < MDL_MEM_CACHE_COUNT) {
        if (mdlData6[i++] != NULL) {
            count = i;
        }
    }
    mdlCallMemGetInfoData = count;
}
#else
INCLUDE_ASM(const s32, "code_00143568", mdlCallMemGetInfo);
#endif

INCLUDE_ASM(const s32, "code_00143568", sbtlCheckEndEscape);
