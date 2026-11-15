#include "common.h"
#include "model.h"
#include "motion.h"
#include "draw3d.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "sdfChip.h"
#include "sdfVu0.h"
#include "mdlManager.h"
#include "sdfQuat.h"
#include "sdfMotion.h"

#define MDL_GROUP_COUNT 8

#define PIB_TAG_PIB0 0x30424950

#define PIB_TAG_P2A 0x413250

#define PIB_TAG_D3P 0x503344

#define PIB_TAG_IPU 0x555049

typedef struct mdlLoadingTag_t
{
    struct mdlLoadingTag_t *next;
    s32 minor;
} mdlLoadingTag_t;

typedef struct
{
    sdfModelTexList_t *texList;
    sdfModelDef_t *modelDefPtr;
    sdfMemHandle_t *modelDefHandle;
    sdfMotionPlayBook_t *playbookPtr;
    sdfMemHandle_t *playbookHandle;
    void *effectData;
    sdfMemHandle_t *effectMemHandle;
    sdfItemArray_t *effectResrcList;
} mdlSetDef_t;

typedef struct
{
    u16 group;
    u16 id;
    u8 resourceListRequested;
    u8 itemsRequested;
    u8 deferred;
    u8 pad7;
    u32 options;
    mdlSetDef_t payload;
} mdlManager_mdlTbMbAbLoadCb_t;

extern s32 mdlManagerWork;
extern dds3ModelResrc_t *mdlResrcMajorList[MDL_GROUP_COUNT];
extern mdlLoadingTag_t *mdlLoadingMajorList[MDL_GROUP_COUNT];
extern s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);
extern s32 WaitSema(s32 sema);
extern s32 SignalSema(s32 sema);
extern void sdfTexListFree(sdfModelTexList_t *aList, s32 aReleaseItems);
extern sdfModelTexList_t *sdfTexListDup(sdfModelTexList_t *aList);
extern sdfModelRootNode_t *sdfModelMake(sdfModelTexList_t *aTexList, sdfModelDef_t *aModelData);
extern void sdfModelRootFree(sdfModelRootNode_t *aRoot, s32 aReleaseNodes, s32 aReleaseMatList);


extern void fileManager(void);
extern void mdlSetEffectColorSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, u32 aColor);
extern void mdlSetEffectScaleSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, f32 aScale);
void mdlUnRegist(s32 aMajor, s32 aMinor);
void mdlUnRegistResrc(dds3ModelResrc_t *resrc_p);
void mdlFree(mdlObj_t *aHandle);
void mdlFreeMotion(mdlObj_t *aHandle, s32 aGroup);
void mdlUnRegistResrcMotion(dds3ModelResrc_t *aResrc, s32 aGroup);

void mdlRegistMotion(s32 aMajor, s32 aMinor, u32 aFlags, s32 aFirstMotion, s32 aGroup, s32 aPlayGroup,
                     sdfMotionPlayBook_t *aData, sdfMemHandle_t *aHandle);

dds3ModelResrc_t *mdlSearchResrc(s32 aMajor, s32 aMinor);

void mdlMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, s32 aType, f32 aBeforeLeng,
               f32 aSmoothLeng);

sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *aHandle, s32 aGroup);
void mdlSetEffectColor(mdlObj_t *aHandle, u32 aColor);

typedef struct fileRequest_t fileRequest_t;

extern void *fileGetLoadPtr(fileRequest_t *fileHandle);
extern sdfMemHandle_t *fileGetMemHandle(fileRequest_t *fileHandle);
extern void fileHandleFree(fileRequest_t *fileHandle);
extern sdfModelDef_t *sdfFixupModelBinary(void *aData);
void TbMbAbLoadEnd(mdlManager_mdlTbMbAbLoadCb_t *tmaCb);
void mdlFreeBlur(mdlObj_t *aHandle);
extern dds3ModelResrc_t *mdlLoadPrepare(u32 group, u32 id);
extern void mdlGenEffect(mdlObj_t *aHandle, s32 aGroup, s32 aPlayGroup);
extern void mdlFreeEffect(dds3ModelEffectCtrlr_t *aCtrlr);

typedef struct
{
    u16 group;
    u16 id;
    s32 arg;
    void *owner;
    void (*done)(u32);
    u32 doneArg;
} mdlManager_mdlFileLoadCb_t;

typedef struct
{
    u8 pad0[0x60];
    struct sdfPacDecodeResrc_t *queueHead;
} filePacRequest_t;

extern void pacFileHandleFree_(void *pacHandle);
saddr_t mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);
void mdlFreeMotionCtrlr(mdlObj_t *aHandle, sdfMotionCtrlr_t *aCtrlr);
extern void sdfPacInit(sdfPacDecodeCb_t *cb, sdfPacCallbackFunc callback);
extern void sdfPacSetStay(sdfPacDecodeCb_t *cb);
extern s32 sdfPacDecode(sdfPacDecodeCb_t *cb, const u8 *data, s32 length);
extern void sdfPacFree(sdfPacDecodeCb_t *cb);
sdfPacDecodeResrc_t *mdlPackSet(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags);
extern filePacRequest_t *pacLoadEx(const char *filename, s32 headOrTail, s32 callback, void *pendCallback, void *callbackParm);
extern void pacSync(filePacRequest_t *pacFileHandle);
void mdlFileCbFree(mdlManager_mdlFileLoadCb_t *cb);
extern void sdfModelEvalTm(sdfModelRootNode_t *aRoot);
extern void sdfModelLinkOt(void *aOt, sdfModelRootNode_t *aRoot);
extern void mdlLinkEffect(mdlObj_t *ctx, dds3ModelEffectCtrlr_t *item);
extern void mdlLinkBlur(mdlObj_t *ctx, sdfGrOt_t **surfaces);
void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

typedef struct
{
    char *texFile;
    char *modelFile;
    char *motionFile;
} dds3ModelFileTable_t;

typedef struct
{
    dds3ModelFileTable_t *entries;
    s32 count;
} mdlFileSlot_t;

typedef struct
{
    u16 major;
    u16 minor;
    u16 scale;
    u16 radius;
} dds3ModelFileIndex_t;

typedef struct
{
    dds3ModelFileIndex_t *entries;
    s32 count;
} mdlFileKindSlot_t;

extern mdlFileKindSlot_t modelFileKindTable[];
extern sdfMemHandle_t *mdlHddFileIndexHandle;
extern sdfMemHandle_t *mdlHddFileListHandle;

typedef struct
{
    char bytes[8];
} mdlPathPrefix_t;

typedef struct
{
    char text[5];
} mdlWorkPrefix_t;

typedef struct
{
    u32 mode;
    u8 stat[0x3C];
    char name[0x100];
    u32 privateWords[2];
} mdlDirectoryEntry_t;

typedef struct mdlHddFileEntry_t
{
    struct mdlHddFileEntry_t *next;
    char name[47];
    u8 available[3];
    s8 extension[3][2];
} mdlHddFileEntry_t;

typedef struct mdlPibRecord_t {
    s32 kind;       /* 0x00: 0xFFFF terminates the record chain */
    s32 nextOffset; /* 0x04: relative byte offset to next record */
    union {
        u32 word;               /* 0x08 */
        struct {
            u16 selectorA;
            u16 selectorB;
        } stream;
    } payload;
    union {
        u32 word;               /* 0x0C */
        struct {
            u16 count;
            u16 partIndex;
        } part;
    } parameter;
    union {
        u32 word10; /* 0x10: kind-four stream parameter */
        struct {
            u16 slotCount; /* Kind 1/2 group records: following entity-slot packets. */
            u16 unk12;
        } half;
    } tail;
} mdlPibRecord_t;

typedef struct mdlPartTagRecord_t {
    u32 tag;         /* 0x00 */
    u8 state;        /* 0x04 */
    u8 opaque05[3];  /* 0x05 */
} mdlPartTagRecord_t;

struct mdlEffectDescriptor_t;

extern char mdlModelPathPrefix[];
extern char mdlPackedBundleExt[];
extern mdlWorkPrefix_t mdlWorkPathPrefix[];
extern const char _ctype_[];
extern void sdfModelCopyStat(sdfModelRootNode_t *aDstRoot, sdfModelRootNode_t *aSrcRoot);
extern u16 mdlPibGetEffectCount(mdlPibRecord_t *record);
extern u16 mdlPibGetMotionCount(mdlPibRecord_t *record);
extern s32 mdlPibGetVersion(mdlPibRecord_t *record);
extern sdfPacDecodeResrc_t *sdfPacFreeResrc(sdfPacDecodeResrc_t *resrc);
extern sdfMatrix4_t sdfViewMatrix;
extern sdf3DScreenParam_t dds3ScreenParam;
extern s32 mdlFileLoad(s32 major, s32 minor, s32 regFlags, const char *filename, void (*callback)(u32), u32 parm);

#define MDL_CHAR_IS_LOWER(c) ((_ctype_ + 1)[(unsigned)(c)] & 2)

static inline s32 FileUpper(s32 c)
{
    return MDL_CHAR_IS_LOWER(c) ? c - 'a' + 'A' : c;
}

static inline void UnpackRGBAWordFloat(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK(rgba, unit);
}

static inline u32 PackDevSlotColor(s32 age, f32 unit) {
    u32 rgba[4];

    VU0_SEQ_MDL_PACK_DEV_SLOT_COLOR(age, unit);
    return rgba[0];
}

void mdlInitManager(void);
s32 mdlCheckLoading(s32 aMajor, s32 aMinor);
void mdlRegLoading(s32 aMajor, s32 aMinor);
void mdlUnRegLoading(s32 aMajor, s32 aMinor);

void mdlRegist(s32 aMajor, s32 aMinor, s32 aFlag,
               sdfModelTexList_t *aTexList, sdfModelDef_t *aData,
               sdfMemHandle_t *aHandle);

void mdlUnRegistAll(void);
sdfMotionCtrlr_t *mdlCreateMotion(mdlObj_t *aHandle, s32 aGroup);
void mdlUnRegistMotion(s32 aMajor, s32 aMinor, s32 aGroup);
void mdlRegistModelSet(s32 aMajor, s32 aMinor, s32 aFlag, mdlSetDef_t *aSet);
dds3ModelResrc_t *mdlWaitLoadEnd(s32 aMajor, s32 aMinor);
void MbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm);
char *mdlSetFileName(char *buff, const char *fname);
dds3ModelResrc_t *mdlLoadPrepare(u32 group, u32 id);
void mdlUnlinkResrc(mdlObj_t *aHandle);

void mdlSetBlur(mdlObj_t *aHandle, s32 interval, s32 count,
                         u32 startColor, u32 endColor);

void mdlLinkBlur(mdlObj_t *ctx, struct sdfGrOt_t **surfaces);
mdlObj_t *mdlLoad(u32 aMajor, u32 aMinor);
void mdlLink(mdlObj_t *aHandle, void *aModelOt);
void mdlHideAllNode(mdlObj_t *aHandle);
void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                         f32 aSmoothLeng);

void mdlSmoothSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                           f32 aSmoothLeng);

s32 mdlGetMotionNum(mdlObj_t *aHandle, s32 aGroup);
u16 mdlGetMotionLengthEx(mdlObj_t *aHandle, s32 aGroup);
s32 mdlGetMotionFrameEx(mdlObj_t *aHandle, s32 aGroup);
s32 mdlChkMotionEndEx(mdlObj_t *aHandle, s32 aGroup);
f32 mdlGetMotionSpeedEx(mdlObj_t *aHandle, s32 aGroup);
void mdlSetMotionSpeedEx(mdlObj_t *aHandle, s32 aGroup, f32 speed);
void mdlGetPosition(mdlObj_t *aHandle);
void mdlSetPosition(mdlObj_t *aHandle);
void mdlGetRotate(mdlObj_t *aHandle);
void mdlSetRotate(mdlObj_t *aHandle);
void mdlGetScale(mdlObj_t *aHandle);
void mdlSetScale(mdlObj_t *aHandle);
u32 mdlGetColor(mdlObj_t *aHandle);
void mdlSetColor(mdlObj_t *aHandle, u32 aColor);
void mdlSetColorWithEffect(mdlObj_t *aHandle, u32 aColor);
void mdlSetEffectScale(mdlObj_t *aHandle, f32 aScale);
void mdlGetScreenOffset(mdlObj_t *aHandle, void *aOfs);
void mdlProjectPoints(mdlObj_t *ctx, f32 (*inputPoints)[4], f32 (*outputPoints)[4], s32 pointCount);
void mdlMotionPause(mdlObj_t *aHandle);
void mdlMotionPauseOff(mdlObj_t *aHandle);
s32 mdlHasMotionEx(mdlObj_t *aHandle, s32 aGroup);
s32 mdlGetMajorNum(mdlObj_t *aHandle);
s32 mdlGetMinorNum(mdlObj_t *aHandle);
s32 mdlGetNumOfMinor(s32 aMajor);
s32 mdlGetNumOfCut(mdlObj_t *aHandle, s32 aGroup);
s32 mdlPackCheck(sdfPacDecodeResrc_t *resrc);
sdfPacDecodeResrc_t *mdlPackSetOld(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags);
void mdlFileLoadCallback(filePacRequest_t *pacHandle, mdlManager_mdlFileLoadCb_t *data);

s32 mdlFileLoad(s32 major, s32 minor, s32 regFlags, const char *filename, void (*callback)(u32),
                u32 parm);

void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);
void mdlFreeFileItem(void *aItem);
extern char mdlModelPathPrefix[8];
extern char mdlPackedBundleExt[4];
extern mdlWorkPrefix_t mdlWorkPathPrefix[1];

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/model.h"

struct mdlEffectDescriptor_t;

void mdlBlendEntryPitchYawAndUpdate(mdlObj_t *aHandle, void *aModelOt, s32 aNode, f32 aPitch, f32 aYaw);

void mdlInitManager(void)
{
    s32 groupIndex;

    mdlManagerWork = sdfCreateSema(1, 0x7F, 0);
    for (groupIndex = 0; groupIndex != MDL_GROUP_COUNT; groupIndex++)
    {
        mdlResrcMajorList[groupIndex] = NULL;
        mdlLoadingMajorList[groupIndex] = NULL;
    }
}

dds3ModelResrc_t *mdlSearchResrc(s32 aMajor, s32 aMinor)
{
    dds3ModelResrc_t *entry;

    entry = mdlResrcMajorList[aMajor];
    while (entry != NULL)
    {
        if (entry->minor == aMinor)
        {
            break;
        }
        entry = entry->next;
    }
    return entry;
}

s32 mdlCheckLoading(s32 aMajor, s32 aMinor)
{
    mdlLoadingTag_t *cursor;

    cursor = mdlLoadingMajorList[aMajor];
    while (cursor != NULL)
    {
        if (cursor->minor == aMinor)
        {
            return 1;
        }
        cursor = cursor->next;
    }
    return 0;
}

void mdlRegLoading(s32 aMajor, s32 aMinor)
{
    mdlLoadingTag_t *entry;
    mdlLoadingTag_t **head;

    entry = sdfChipAllocDirect(sizeof(mdlLoadingTag_t));
    head = &mdlLoadingMajorList[aMajor];
    entry->minor = aMinor;
    entry->next = *head;
    *head = entry;
}

void mdlUnRegLoading(s32 aMajor, s32 aMinor)
{
    mdlLoadingTag_t **link;
    mdlLoadingTag_t *cursor;

    link = &mdlLoadingMajorList[aMajor];
    cursor = *link;
    if (cursor == NULL)
    {
        return;
    }
    do
    {
        if (cursor->minor == aMinor)
        {
            *link = cursor->next;
            sdfChipFreeDirect(cursor);
            break;
        }
        else
        {
            link = &cursor->next;
            cursor = cursor->next;
        }
    } while (cursor != NULL);
}

void mdlRegist(s32 aMajor, s32 aMinor, s32 aFlag,
               sdfModelTexList_t *aTexList, sdfModelDef_t *aData,
               sdfMemHandle_t *aHandle)
{
    dds3ModelResrc_t *groupNode;
    dds3ModelResrc_t *previousHead;
    s32 slotIndex;

    mdlUnRegist(aMajor, aMinor);
    groupNode = sdfChipAllocDirect(sizeof(dds3ModelResrc_t));
    previousHead = mdlResrcMajorList[aMajor];
    if (previousHead != NULL)
    {
        previousHead->prev = groupNode;
    }
    mdlResrcMajorList[aMajor] = groupNode;
    groupNode->next = previousHead;
    groupNode->major = aMajor;
    groupNode->minor = aMinor;
    groupNode->texList = aTexList;
    groupNode->modelDefPtr = aData;
    groupNode->modelDefHandle = aHandle;
    groupNode->prev = NULL;
    groupNode->models = NULL;
    for (slotIndex = 0; slotIndex != MDL_SLOT_COUNT; slotIndex++)
    {
        groupNode->playbooks[slotIndex].flags = 0;
        groupNode->playbooks[slotIndex].ptr = NULL;
        groupNode->playbooks[slotIndex].handle = NULL;
    }
    groupNode->autoFreeFlag = aFlag & 1;
    groupNode->effectMemHandle = NULL;
    groupNode->effectData = NULL;
    groupNode->effectResrcList = NULL;
    groupNode->scale = 1.0f;
    groupNode->radius = 100.0f;
}

void mdlUnRegistResrc(dds3ModelResrc_t *resrc_p)
{
    dds3ModelResrc_t *previousNode;
    dds3ModelResrc_t *nextNode;
    u8 ownsResources;
    s32 slotIndex;

    if (resrc_p == NULL)
    {
        return;
    }
    previousNode = resrc_p->prev;
    nextNode = resrc_p->next;
    if (previousNode == NULL)
    {
        mdlResrcMajorList[resrc_p->major] = nextNode;
    }
    else
    {
        previousNode->next = nextNode;
    }
    if (nextNode != NULL)
    {
        nextNode->prev = previousNode;
    }
    ownsResources = resrc_p->autoFreeFlag;
    resrc_p->autoFreeFlag = 0;
    if (resrc_p->models != NULL)
    {
        do
        {
            mdlFree(resrc_p->models);
        } while (resrc_p->models != NULL);
    }
    if (ownsResources != 0)
    {
        sdfTexListFree(resrc_p->texList, 1);
        sdfMemFreeDelayedDirect(resrc_p->modelDefHandle);
        for (slotIndex = 0; slotIndex != MDL_SLOT_COUNT; slotIndex++)
        {
            if (resrc_p->playbooks[slotIndex].handle != NULL)
            {
                sdfMemFreeDirect(resrc_p->playbooks[slotIndex].handle);
            }
        }
    }
    mdlEffectResrcListFree(resrc_p->effectResrcList);
    sdfMemFreeDirect(resrc_p->effectMemHandle);
    sdfChipFreeDirect(resrc_p);
}

void mdlUnRegist(s32 aMajor, s32 aMinor)
{
    mdlUnRegistResrc(mdlSearchResrc(aMajor, aMinor));
}

void mdlUnRegistAll(void)
{
    u32 groupIndex;
    dds3ModelResrc_t **groupHead;
    dds3ModelResrc_t *groupNode;
    dds3ModelResrc_t *nextNode;

    groupIndex = 0;
    groupHead = mdlResrcMajorList;
    do
    {
        groupNode = *groupHead;
        while (groupNode != NULL)
        {
            nextNode = groupNode->next;
            mdlUnRegistResrc(groupNode);
            groupNode = nextNode;
        }
        groupIndex++;
        groupHead++;
    } while (groupIndex < MDL_GROUP_COUNT);
}

sdfMotionCtrlr_t *mdlCreateMotion(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionPlayBook_t *motionTable;
    s16 motionSlotIndex;
    sdfMotionCtrlr_t *object;

    motionTable = aHandle->sub->playbooks[aGroup].ptr;
    motionSlotIndex = aHandle->sub->playbooks[aGroup].playGroup;
    object = sdfMotionCtrlrNew(aHandle->inner, motionTable);
    object->userId = aGroup;
    aHandle->slots[motionSlotIndex] = object;
    object->userId2 = motionSlotIndex;
    if (motionSlotIndex == 0)
    {
        aHandle->first = object;
    }
    return object;
}

void mdlFreeMotionCtrlr(mdlObj_t *aHandle, sdfMotionCtrlr_t *aCtrlr)
{
    if (aHandle->slots[aCtrlr->userId2] == aCtrlr)
    {
        aHandle->slots[aCtrlr->userId2] = NULL;
    }
    sdfMotionCtrlrFree(aCtrlr);
}

void mdlFreeMotion(mdlObj_t *aHandle, s32 aGroup) {
    sdfMotionCtrlr_t *node = aHandle->inner->motion;

    while (node != NULL) {
        if (node->userId == aGroup) {
            mdlFreeMotionCtrlr(aHandle, node);
            break;
        }
        node = node->next;
    }
}

void mdlUnRegistResrcMotion(dds3ModelResrc_t *aResrc, s32 aGroup)
{
    mdlObj_t *ctx;

    if (aResrc != NULL)
    {
        if (aResrc->playbooks[aGroup].ptr == NULL)
        {
            return;
        }
        for (ctx = aResrc->models; ctx != NULL; ctx = ctx->next)
        {
            mdlFreeMotion(ctx, aGroup);
        }
        if (aResrc->autoFreeFlag != 0)
        {
            if (aResrc->playbooks[aGroup].handle != NULL)
            {
                sdfMemFreeDirect(aResrc->playbooks[aGroup].handle);
            }
        }
        aResrc->playbooks[aGroup].ptr = NULL;
        aResrc->playbooks[aGroup].handle = NULL;
    }
}

void mdlUnRegistMotion(s32 aMajor, s32 aMinor, s32 aGroup)
{
    dds3ModelResrc_t *entity;

    entity = mdlSearchResrc(aMajor, aMinor);
    mdlUnRegistResrcMotion(entity, aGroup);
}

void mdlRegistMotion(s32 aMajor, s32 aMinor, u32 aFlags, s32 aFirstMotion, s32 aGroup, s32 aPlayGroup,
                     sdfMotionPlayBook_t *aData, sdfMemHandle_t *aHandle)
{
    dds3ModelResrc_t *owner;
    dds3ModelResrcPlayBook_t *slot;

    owner = mdlSearchResrc(aMajor, aMinor);
    mdlUnRegistResrcMotion(owner, aGroup);
    slot = &owner->playbooks[aGroup];
    slot->playGroup = aPlayGroup;
    slot->firstMotion = aFirstMotion;
    slot->ptr = aData;
    slot->handle = aHandle;
    slot->flags = 0;
    if (aFlags & 0x100)
    {
        slot->flags = 1;
    }
    if (aFlags & 0x200)
    {
        slot->flags |= 2;
    }
}

void mdlRegistModelSet(s32 aMajor, s32 aMinor, s32 aFlag, mdlSetDef_t *aSet)
{
    dds3ModelResrc_t *entity;

    mdlRegist(aMajor, aMinor, aFlag, aSet->texList, aSet->modelDefPtr, aSet->modelDefHandle);
    if (aSet->playbookPtr != NULL)
    {
        mdlRegistMotion(aMajor, aMinor, aFlag, 0, 0, 0, aSet->playbookPtr, aSet->playbookHandle);
    }
    if (aSet->effectData != NULL)
    {
        entity = mdlSearchResrc(aMajor, aMinor);
        entity->effectData = aSet->effectData;
        entity->effectMemHandle = aSet->effectMemHandle;
        entity->effectResrcList = aSet->effectResrcList;
    }
}

dds3ModelResrc_t *mdlWaitLoadEnd(s32 aMajor, s32 aMinor)
{
    while (mdlCheckLoading(aMajor, aMinor))
    {
        fileManager();
    }
    return mdlSearchResrc(aMajor, aMinor);
}

void TbMbAbLoadEnd(mdlManager_mdlTbMbAbLoadCb_t *tmaCb)
{
    mdlRegistModelSet(tmaCb->group, tmaCb->id, tmaCb->options, &tmaCb->payload);
    WaitSema(mdlManagerWork);
    mdlUnRegLoading(tmaCb->group, tmaCb->id);
    SignalSema(mdlManagerWork);
    sdfChipFreeDirect(tmaCb);
}

void MbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm)
{
    parm->payload.modelDefHandle = fileGetMemHandle(fileHandle);
    parm->payload.modelDefPtr = sdfFixupModelBinary(fileGetLoadPtr(fileHandle));
    fileHandleFree(fileHandle);
    if (parm->deferred == 0)
    {
        TbMbAbLoadEnd(parm);
    }
}

char *mdlSetFileName(char *buff, const char *fname) {
    *(mdlPathPrefix_t *)buff = *(mdlPathPrefix_t *)mdlModelPathPrefix;
    return strcat(buff, fname);
}

INCLUDE_ASM(const s32, "model/mdlManager", mdlCallWaitSema);

dds3ModelResrc_t *mdlLoadPrepare(u32 group, u32 id)
{
    return EE_ADDR_AS(dds3ModelResrc_t *, mdlPrepare(group, id, 1));
}

void mdlUnlinkResrc(mdlObj_t *aHandle)
{
    mdlObj_t *nextContext;
    mdlObj_t *previousContext;
    dds3ModelResrc_t *group;

    nextContext = aHandle->next;
    previousContext = aHandle->prev;
    if (nextContext != NULL)
    {
        nextContext->prev = previousContext;
    }
    if (previousContext != NULL)
    {
        previousContext->next = nextContext;
    }
    else
    {
        group = aHandle->sub;
        if (nextContext != NULL)
        {
            group->models = nextContext;
        }
        else
        {
            group->models = NULL;
            if (group->autoFreeFlag != 0)
            {
                mdlUnRegistResrc(group);
            }
        }
    }
}

void mdlSetBlur(mdlObj_t *aHandle, s32 interval, s32 count,
                         u32 startColor, u32 endColor) {
    dds3ModelBlurCb_t *deviceList;
    f32 inverseSpan;
    /* Both retail versions home the packed colors 16 bytes apart. */
    u32 startCarrier[4];
    u32 endCarrier[4];

    mdlFreeBlur(aHandle);
    if (count > 0) {
        deviceList = sdfChipAllocZeroDirect(sizeof(dds3ModelBlurCb_t));
        deviceList->intervalCount = 1;
        aHandle->devList = deviceList;
        deviceList->spawnCount = count;
        deviceList->count = count;
        deviceList->interval = SDF_FRAMES(interval);
        /* Final argument use: see docs/idioms.md, "Final argument use: mdlSetBlur". */
        deviceList->keepCount = count * SDF_FRAMES(interval);
        inverseSpan = 1.0f / deviceList->keepCount;
        startCarrier[0] = endColor;
        EE_MMI_RGBA_UNPACK(startCarrier, 0.0078125f);
        SDF_VU0_STOREVECTOR(deviceList->colorBase);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        endCarrier[0] = startColor;
        EE_MMI_RGBA_UNPACK(endCarrier, 0.0078125f);
        SDF_VU0_SUBVECTOR_EXTENDED();
        SDF_VU0_SCALEVECTOR_V0(inverseSpan);
        SDF_VU0_STOREVECTOR(deviceList->colorDelta);
    }
}

void mdlFreeBlur(mdlObj_t *aHandle)
{
    dds3ModelBlurCb_t *list;
    dds3ModelBlurInstance_t *slot;
    dds3ModelBlurInstance_t *next;

    list = aHandle->devList;
    if (list == NULL)
    {
        return;
    }
    next = list->instants;
    while (next != NULL)
    {
        slot = next;
        next = next->next;
        sdfModelRootFree(slot->modelobj, 1, 1);
        sdfChipFreeDirect(slot);
    }
    sdfChipFreeDirect(list);
    aHandle->devList = NULL;
}

void mdlLinkBlur(mdlObj_t *ctx, struct sdfGrOt_t **surfaces) {
    dds3ModelBlurCb_t *list = ctx->devList;
    sdfModelRootNode_t *original = ctx->inner;
    dds3ModelBlurInstance_t *slot;
    sdfModelRootNode_t *model;
    sdfVector4_t tint;
    s32 remaining;
    u32 color;

    if (list->spawnCount != 0 && --list->intervalCount == 0) {
        list->spawnCount--;
        list->intervalCount = list->interval;
        slot = sdfChipAllocDirect(sizeof(dds3ModelBlurInstance_t));
        model = sdfModelMake(ctx->sub->texList, ctx->sub->modelDefPtr);
        /* Finish initialization before publishing the new list head. */
        slot->next = list->instants;
        slot->modelobj = model;
        slot->keepCount = 1;
        model->zBias = 1000.0f;
        list->instants = slot;
    }
    UnpackRGBAWordFloat(original->color, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR(&tint);
    slot = list->instants;
    while (slot != NULL) {
        remaining = slot->keepCount;
        model = slot->modelobj;
        remaining--;
        if (remaining == 0) {
            remaining = list->keepCount;
            sdfModelCopyStat(model, original);
        } else {
            sdfModelEvalTm(model);
        }
        /* Both callbacks complete before the countdown is published. */
        slot->keepCount = remaining;
        SDF_VU0_LOADVECTOR_BARRIER(list->colorBase);
        SDF_VU0_LOADVECTOR_B(list->colorDelta);
        SDF_VU0_LOADVECTOR_C(&tint);
        color = PackDevSlotColor(remaining - 1, 128.0f);
        model->color = color;
        sdfModelLinkOt(surfaces, model);
        slot = slot->next;
    }
}

mdlObj_t *mdlLoad(u32 aMajor, u32 aMinor)
{
    dds3ModelResrc_t *node;
    mdlObj_t *ctx;
    mdlObj_t *head;
    sdfModelRootNode_t *inner;
    f32 modelScale;
    sdfVector4_t scale;
    s32 searchId;

    node = mdlLoadPrepare(aMajor, aMinor);
    ctx = sdfChipAllocZeroDirect(sizeof(mdlObj_t));
    ctx->sub = node;
    head = node->models;
    ctx->next = head;
    if (head != NULL)
    {
        head->prev = ctx;
    }
    node->models = ctx;
    inner = sdfModelMake(sdfTexListDup(node->texList), node->modelDefPtr);
    modelScale = node->scale;
    ctx->inner = inner;
    scale.x = modelScale;
    scale.y = modelScale;
    scale.z = modelScale;
    scale.w = 1.0f;
    SDF_VU0_COPYVECTOR(&inner->scale, &scale);
    ctx->objectRadius = node->radius;
    ctx->current.h.id = -1;
    ctx->current.h.motionIndex = -1;
    for (searchId = 0; searchId != MDL_SLOT_COUNT; searchId++)
    {
        if (node->playbooks[searchId].ptr != NULL && (node->playbooks[searchId].flags & 1))
        {
            mdlCreateMotion(ctx, searchId);
            if (searchId > 0)
            {
                if (node->playbooks[searchId].firstMotion >= 0)
                {
                    mdlLoopMotion(ctx, searchId, node->playbooks[searchId].firstMotion);
                }
            }
        }
    }
    ctx->resourceItems = NULL;
    mdlGenEffect(ctx, -1, -1);
    return ctx;
}

void mdlFree(mdlObj_t *aHandle)
{
    sdfModelRootNode_t *inner;
    dds3ModelEffectCtrlr_t *item;
    dds3ModelEffectCtrlr_t *next;

    inner = aHandle->inner;
    while (inner->motion != NULL)
    {
        sdfMotionCtrlrFree(inner->motion);
    }
    sdfTexListFree(inner->texList, 1);
    for (item = aHandle->resourceItems; item != NULL; item = next)
    {
        next = item->next;
        mdlFreeEffect(item);
    }
    mdlFreeBlur(aHandle);
    sdfModelRootFree(inner, 1, 1);
    mdlUnlinkResrc(aHandle);
    sdfChipFreeDirect(aHandle);
}

void mdlLink(mdlObj_t *aHandle, void *aModelOt)
{
    sdfMotionCtrlr_t **slot;
    sdfModelRootNode_t *inner;
    dds3ModelEffectCtrlr_t *item;
    s32 i;

    slot = aHandle->slots;
    for (i = 0; i != 4; i++)
    {
        if (*slot != NULL)
        {
            if ((*slot)->stat != 0)
            {
                sdfMotionNext(*slot);
            }
        }
        slot++;
    }
    if (aHandle->flags & 1)
    {
        return;
    }
    inner = aHandle->inner;
    sdfModelEvalTm(inner);
    sdfModelLinkOt(aModelOt, inner);
    if (aHandle->flags & 2)
    {
        return;
    }
    if (aHandle->flags & 4)
    {
        if ((inner->flags & 0x10) == 0)
        {
            return;
        }
    }
    for (item = aHandle->resourceItems; item != NULL; item = item->next)
    {
        mdlLinkEffect(aHandle, item);
    }
    if (aHandle->devList == NULL)
    {
        return;
    }
    mdlLinkBlur(aHandle, aModelOt);
}

INCLUDE_ASM(const s32, "model/mdlManager", LinkMotion);

/* mdlLink, first turning node aNode's local rotation towards aPitch and aYaw
 * (degrees) by abs(aPitch) / 25, at most one. aNode -1 skips the turn. */
void mdlBlendEntryPitchYawAndUpdate(mdlObj_t *aHandle, void *aModelOt, s32 aNode, f32 aPitch, f32 aYaw)
{
    dds_sdfModelRootNode_t *inner;
    sdfModelNode_t *node = NULL;
    f32 target[4][4];
    f32 magnitude;
    f32 weight;
    f32 rest;
    dds3ModelEffectCtrlr_t *item;
    s32 i;

    inner = aHandle->inner;
    if (aNode != -1)
    {
        node = inner->nodeList->items[aNode];
        LoadIdentity();
        RotateMatrixX(aPitch * 0.017453293f);
        RotateMatrixY(aYaw * 0.017453293f);
        SDF_VU0_STOREMATRIX(target);
    }
    for (i = 0; i != 4; i++)
    {
        if (aHandle->slots[i] != NULL)
        {
            if (aHandle->slots[i]->stat != 0)
            {
                sdfMotionNext(aHandle->slots[i]);
            }
        }
    }
    magnitude = aPitch;
    if (aNode != -1)
    {
        if (magnitude < 0.0f)
        {
            magnitude = -magnitude;
        }
        weight = 1.0f;
        if (magnitude < 25.0f)
        {
            weight = magnitude / 25.0f;
        }
        rest = 1.0f - weight;
        node->localNoScaledTm.m[0] = target[0][0] * weight + node->localNoScaledTm.m[0] * rest;
        node->localNoScaledTm.m[1] = target[0][1] * weight + node->localNoScaledTm.m[1] * rest;
        node->localNoScaledTm.m[2] = target[0][2] * weight + node->localNoScaledTm.m[2] * rest;
        node->localNoScaledTm.m[4] = target[1][0] * weight + node->localNoScaledTm.m[4] * rest;
        node->localNoScaledTm.m[5] = target[1][1] * weight + node->localNoScaledTm.m[5] * rest;
        node->localNoScaledTm.m[6] = target[1][2] * weight + node->localNoScaledTm.m[6] * rest;
        node->localNoScaledTm.m[8] = target[2][0] * weight + node->localNoScaledTm.m[8] * rest;
        node->localNoScaledTm.m[9] = target[2][1] * weight + node->localNoScaledTm.m[9] * rest;
        node->localNoScaledTm.m[10] = target[2][2] * weight + node->localNoScaledTm.m[10] * rest;
    }
    if (aHandle->flags & 1)
    {
        return;
    }
    inner = aHandle->inner;
    sdfModelEvalTm(inner);
    sdfModelLinkOt(aModelOt, inner);
    if (aHandle->flags & 2)
    {
        return;
    }
    if (aHandle->flags & 4)
    {
        if ((inner->flags & 0x10) == 0)
        {
            return;
        }
    }
    for (item = aHandle->resourceItems; item != NULL; item = item->next)
    {
        mdlLinkEffect(aHandle, item);
    }
    if (aHandle->devList == NULL)
    {
        return;
    }
    mdlLinkBlur(aHandle, aModelOt);
}

void mdlHideAllNode(mdlObj_t *aHandle)
{
    sdfModelNode_t **items;
    s32 count;
    s32 i;

    count = aHandle->inner->nodeList->count;
    items = aHandle->inner->nodeList->items;
    for (i = 0; i < count; i++) {
        items[i]->flags = 1;
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", mdlMotion);

void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber)
{
    mdlMotion(aHandle, aGroup, aNumber, 1, 0.0f, 0.0f);
}

void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber)
{
    mdlMotion(aHandle, aGroup, aNumber, 0, 0.0f, 0.0f);
}

void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                         f32 aSmoothLeng)
{
    mdlMotion(aHandle, aGroup, aNumber, 1, aBeforeLeng, aSmoothLeng);
}

void mdlSmoothSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                           f32 aSmoothLeng)
{
    mdlMotion(aHandle, aGroup, aNumber, 0, aBeforeLeng, aSmoothLeng);
}

sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *node;

    for (node = aHandle->inner->motion; node != NULL; node = node->next)
    {
        if (node->userId == aGroup)
        {
            return node;
        }
    }
    return NULL;
}

s32 mdlGetMotionNum(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched == NULL)
    {
        return -1;
    }
    return matched->cutNumber;
}

u16 mdlGetMotionLengthEx(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched == NULL)
    {
        return 0;
    }
    return matched->frameLength;
}

s32 mdlGetMotionFrameEx(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched == NULL)
    {
        return 0;
    }
    return (s32)matched->frame;
}

s32 mdlChkMotionEndEx(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched == NULL)
    {
        return 2;
    }
    return matched->stat == 5;
}

f32 mdlGetMotionSpeedEx(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;
    f32 result;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    result = 0.0f;
    if (matched != NULL)
    {
        result = matched->speed;
    }
    return result;
}

void mdlSetMotionSpeedEx(mdlObj_t *aHandle, s32 aGroup, f32 speed)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched != NULL)
    {
        matched->speed = speed;
    }
}

void mdlGetPosition(mdlObj_t *aHandle)
{
    SDF_VU0_LOADVECTOR_BARRIER(&aHandle->inner->tm.m[12]);
}

void mdlSetPosition(mdlObj_t *aHandle)
{
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&aHandle->inner->tm.m[12]);
}

void mdlGetRotate(mdlObj_t *aHandle)
{
    SDF_VU0_LOADVECTOR_BARRIER(&aHandle->inner->rotate);
}

void mdlSetRotate(mdlObj_t *aHandle)
{
    SDF_VU0_STOREVECTOR(&aHandle->inner->rotate);
    ToMatrix();
    SDF_VU0_STORE_ROW0(&aHandle->inner->tm.m[0]);
    SDF_VU0_STORE_ROW1(&aHandle->inner->tm.m[4]);
    SDF_VU0_STORE_ROW2(&aHandle->inner->tm.m[8]);
}

void mdlGetScale(mdlObj_t *aHandle)
{
    SDF_VU0_LOADVECTOR_BARRIER(&aHandle->inner->scale);
}

void mdlSetScale(mdlObj_t *aHandle)
{
    SDF_VU0_STOREVECTOR(&aHandle->inner->scale);
}

u32 mdlGetColor(mdlObj_t *aHandle)
{
    return aHandle->inner->color;
}

void mdlSetEffectColor(mdlObj_t *aHandle, u32 aColor)
{
    dds3ModelEffectCtrlr_t *item;

    for (item = aHandle->resourceItems; item != NULL; item = item->next)
    {
        mdlSetEffectColorSub(aHandle, item, aColor);
    }
}

void mdlSetColor(mdlObj_t *aHandle, u32 aColor)
{
    aHandle->inner->color = aColor;
    mdlSetEffectColor(aHandle, (aColor & 0xFF000000) | 0x808080);
}

void mdlSetColorWithEffect(mdlObj_t *aHandle, u32 aColor)
{
    aHandle->inner->color = aColor;
    mdlSetEffectColor(aHandle, aColor);
}

void mdlSetEffectScale(mdlObj_t *aHandle, f32 aScale)
{
    dds3ModelEffectCtrlr_t *item;

    for (item = aHandle->resourceItems; item != NULL; item = item->next)
    {
        mdlSetEffectScaleSub(aHandle, item, aScale);
    }
}

/* vu0 routine: project the point through the view and model matrices into vf10. */
void mdlGetScreenOffset(mdlObj_t *aHandle, void *aOfs)
{
    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADMATRIX(&aHandle->inner->tm);
    SDF_VU0_SCALE_XYZ();
    MultMatrixRevNoArg();
    SDF_VU0_LOADVECTOR_BARRIER(aOfs);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
}

/* vu0 routine: project an array of points and store their screen coordinates. */
void mdlProjectPoints(mdlObj_t *ctx, f32 (*inputPoints)[4], f32 (*outputPoints)[4], s32 pointCount)
{
    s32 pointIndex;

    SDF_VU0_LOADMATRIX(&ctx->inner->tm);
    SDF_VU0_LOADVECTOR_BARRIER(&ctx->inner->scale);
    SDF_VU0_SCALE_XYZ();
    MultMatrixRev(&sdfViewMatrix);
    MultMatrixRev(&dds3ScreenParam.TM);
    for (pointIndex = 0; pointIndex < pointCount; pointIndex++) {
        SDF_VU0_LOADVECTOR_BARRIER(inputPoints[pointIndex]);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_PERSVECTOR();
        SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(outputPoints[pointIndex]);
    }
}

void mdlMotionPause(mdlObj_t *aHandle)
{
    sdfMotionCtrlr_t *node;

    for (node = aHandle->inner->motion; node != NULL; node = node->next)
    {
        sdfMotionPause(node);
    }
}

void mdlMotionPauseOff(mdlObj_t *aHandle)
{
    sdfMotionCtrlr_t *node;

    for (node = aHandle->inner->motion; node != NULL; node = node->next)
    {
        sdfMotionPauseOff(node);
    }
}

s32 mdlHasMotionEx(mdlObj_t *aHandle, s32 aGroup)
{
    return mdlGetMotionCtrlr(aHandle, aGroup) != NULL;
}

s32 mdlGetMajorNum(mdlObj_t *aHandle)
{
    return aHandle->sub->major;
}

s32 mdlGetMinorNum(mdlObj_t *aHandle)
{
    return aHandle->sub->minor;
}

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNumOfMajor);

s32 mdlGetNumOfMinor(s32 aMajor)
{
    return modelFileKindTable[aMajor].count;
}

s32 mdlGetNumOfCut(mdlObj_t *aHandle, s32 aGroup)
{
    sdfMotionCtrlr_t *matched;

    matched = mdlGetMotionCtrlr(aHandle, aGroup);
    if (matched == NULL)
    {
        return 0;
    }
    return matched->playbook->numOfCut;
}

void mdlSetClutColor(mdlObj_t *ctx, s32 color, f32 ratio)
{
    sdfTexListSetClutColor((sdfItemArray_t *)ctx->inner->texList, color, ratio);
}

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetFilebase);

s32 mdlPackCheck(sdfPacDecodeResrc_t *resrc)
{
    s32 result;

    result = 0;
    if (((sdfPacStdBlockHeader_t *)resrc->header)->fileType == DDS3_FT_EFFECT)
    {
        result = ((sdfPacStdBlockHeader_t *)resrc->header)->magicCode == PIB_TAG_PIB0;
    }
    return result;
}

sdfPacDecodeResrc_t *mdlPackSetOld(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags) {
    mdlSetDef_t setup;
    sdfItemArray_t *parts;
    s32 count;

    memset(&setup, 0, sizeof(setup));
    parts = NULL;
    while (resrc != NULL) {
        switch (((sdfPacStdBlockHeader_t *)resrc->header)->fileType) {
        case DDS3_FT_EFFECT:
            switch (((sdfPacStdBlockHeader_t *)resrc->header)->magicCode) {
            case PIB_TAG_PIB0:
                setup.effectData = resrc->data;
                setup.effectMemHandle = (sdfMemHandle_t *)resrc->resrc;
                count = mdlPibGetEffectCount((mdlPibRecord_t *)resrc->data);
                if (count > 0) {
                    parts = mdlEffectResrcListNew(count);
                    setup.effectResrcList = parts;
                }
                break;
            case PIB_TAG_P2A:
                mdlCreateEffectResrc_P2A(parts, resrc->data);
                sdfMemFreeDirect((sdfMemHandle_t *)resrc->resrc);
                break;
            case PIB_TAG_D3P:
                mdlCreateEffectResrc_D3P(parts, (struct mdlEffectDescriptor_t *)resrc->data);
                sdfMemFreeDirect((sdfMemHandle_t *)resrc->resrc);
                break;
            }
            break;
        case DDS3_FT_TEXLIST:
            setup.texList = (sdfModelTexList_t *)resrc->resrc;
            break;
        case DDS3_FT_MODEL:
            if (setup.modelDefHandle == NULL) {
                setup.modelDefHandle = (sdfMemHandle_t *)resrc->resrc;
                setup.modelDefPtr = (sdfModelDef_t *)resrc->data;
            }
            break;
        case DDS3_FT_MOTION:
            if (setup.playbookHandle == NULL) {
                setup.playbookHandle = (sdfMemHandle_t *)resrc->resrc;
                setup.playbookPtr = (sdfMotionPlayBook_t *)resrc->data;
            }
            break;
        }
        resrc = sdfPacFreeResrc(resrc);
    }
    mdlRegistModelSet(major, minor, regFlags, &setup);
    return resrc;
}

sdfPacDecodeResrc_t *mdlPackSet(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags) {
    mdlSetDef_t groupSetup;
    mdlPartTagRecord_t partTagRecord;
    sdfPacStdBlockHeader_t *packetHeader;
    mdlPibRecord_t *record;
    mdlPibRecord_t *slotRecord;
    s32 partCount;
    sdfItemArray_t *partList;
    s32 slotCount;
    s32 objectSize;
    s32 remainingParts;
    s32 payloadValue;

    if (mdlPackCheck(resrc) == 0) {
        return mdlPackSetOld(resrc, major, minor, regFlags);
    }
    record = (mdlPibRecord_t *)resrc->data;
    payloadValue = mdlPibGetVersion(record);
    if (payloadValue <= 0) {
        return mdlPackSetOld(resrc, major, minor, regFlags);
    }

    memset(&groupSetup, 0, sizeof(groupSetup));
    groupSetup.effectData = record;
    groupSetup.effectMemHandle = (sdfMemHandle_t *)resrc->resrc;
    resrc = sdfPacFreeResrc(resrc);
    partCount = mdlPibGetEffectCount(record);
    remainingParts = partCount;
    if (partCount > 0) {
        partList = mdlEffectResrcListNew(partCount);
        groupSetup.effectResrcList = partList;
    }
    if (remainingParts > 0) {
        do {
            switch (((sdfPacStdBlockHeader_t *)resrc->header)->magicCode) {
            case PIB_TAG_P2A:
                mdlCreateEffectResrc_P2A(partList, resrc->data);
                sdfMemFreeDirect((sdfMemHandle_t *)resrc->resrc);
                break;
            case PIB_TAG_D3P:
                mdlCreateEffectResrc_D3P(partList, (struct mdlEffectDescriptor_t *)resrc->data);
                sdfMemFreeDirect((sdfMemHandle_t *)resrc->resrc);
                break;
            case PIB_TAG_IPU:
                objectSize = ((sdfPacStdBlockHeader_t *)resrc->header)->realSize;
                if (objectSize == 0) {
                    objectSize = ((sdfPacStdBlockHeader_t *)resrc->header)->size
                               + (((sdfPacStdBlockHeader_t *)resrc->header)->flags & 0xF0) - 0x10;
                }
                mdlCreateEffectResrc_IPU(partList, resrc->data,
                                    (sdfMemHandle_t *)resrc->resrc, objectSize);
                break;
            default:
                /* Retained metadata stores: SMT3 1DE078/1DE07C and
                 * DDS1 2188E0/2188E4 (same otherwise-unused local record). */
                packetHeader = (sdfPacStdBlockHeader_t *)resrc->header;
                partTagRecord.tag = packetHeader->magicCode;
                partTagRecord.state = 0;
                break;
            }
            resrc = sdfPacFreeResrc(resrc);
            remainingParts--;
        } while (remainingParts != 0);
    }

    if (((sdfPacStdBlockHeader_t *)resrc->header)->fileType == DDS3_FT_TEXLIST) {
        groupSetup.texList = (sdfModelTexList_t *)resrc->resrc;
        resrc = sdfPacFreeResrc(resrc);
    }
    groupSetup.modelDefHandle = (sdfMemHandle_t *)resrc->resrc;
    groupSetup.modelDefPtr = (sdfModelDef_t *)resrc->data;
    resrc = sdfPacFreeResrc(resrc);
    mdlRegistModelSet(major, minor, regFlags, &groupSetup);

    slotCount = mdlPibGetMotionCount(record);
    while (slotCount > 0) {
        slotRecord = (mdlPibRecord_t *)(resrc->data - (u32)&((mdlPibRecord_t *)0)->tail);
        mdlRegistMotion(major, minor, regFlags, (s16)slotRecord->parameter.word,
                                      slotRecord->payload.stream.selectorA,
                                      slotRecord->payload.stream.selectorB,
                                      (sdfMotionPlayBook_t *)resrc->data,
                                      (sdfMemHandle_t *)resrc->resrc);
        resrc = sdfPacFreeResrc(resrc);
        slotCount--;
    }
    return resrc;
}

void mdlFileCbFree(mdlManager_mdlFileLoadCb_t *cb)
{
    pacFileHandleFree_(cb->owner);
    sdfChipFreeDirect(cb);
}

void mdlFileLoadCallback(filePacRequest_t *pacHandle, mdlManager_mdlFileLoadCb_t *data)
{
    data->owner = pacHandle;
    mdlPackSet(pacHandle->queueHead, data->group, data->id, data->arg);
    WaitSema(mdlManagerWork);
    mdlUnRegLoading(data->group, data->id);
    SignalSema(mdlManagerWork);
    if (data->done != NULL)
    {
        data->done(data->doneArg);
        mdlFileCbFree(data);
    }
}

s32 mdlFileLoad(s32 major, s32 minor, s32 regFlags, const char *filename, void (*callback)(u32),
                u32 parm)
{
    mdlManager_mdlFileLoadCb_t *job;
    filePacRequest_t *request;

    job = sdfChipAllocZeroDirect(sizeof(mdlManager_mdlFileLoadCb_t));
    job->group = major;
    job->id = minor;
    job->arg = regFlags;
    job->doneArg = parm;
    job->done = callback;
    request = pacLoadEx(filename, 0, 0, mdlFileLoadCallback, job);
    job->owner = request;
    if (callback == NULL)
    {
        pacSync(request);
        mdlFileCbFree(job);
    }
    return 0;
}

void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng)
{
    sdfPacDecodeCb_t state;

    sdfPacInit(&state, 0);
    if (regFlags & 2)
    {
        sdfPacSetStay(&state);
    }
    sdfPacDecode(&state, buff, leng);
    mdlPackSet(state.resrcHead, major, minor, regFlags);
    sdfPacFree(&state);
}

void mdlFreeFileItem(void *aItem)
{
    sdfChipFreeDirect(aItem);
}

INCLUDE_ASM(const s32, "model/mdlManager", mdlReleaseHddFileList);

INCLUDE_ASM(const s32, "model/mdlManager", mdlMakeHddFileListDummy);

INCLUDE_ASM(const s32, "model/mdlManager", mdlMakeHddFileList);
char mdlModelPathPrefix[8] = "/model/";

INCLUDE_SDATA(const s32, "model/mdlManager", mdlPackedBundleExt);

INCLUDE_SDATA(const s32, "model/mdlManager", mdlHddFileIndexHandle);

INCLUDE_SDATA(const s32, "model/mdlManager", mdlHddFileListHandle);

INCLUDE_SDATA(const s32, "model/mdlManager", mdlWorkPathPrefix);

