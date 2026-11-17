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
#include "sdk_libkernl.h"
#include "sdfMotion.h"
#include "sdfSystem.h"
#include "sdfModel.h"
#include "mdlEffect.h"
#include "sdfPacDecode.h"
#include "sdf_model_packet_callback.h"
#include "fileManager.h"

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
extern void sdfTexListFree(sdfModelTexList_t *aList, s32 aReleaseItems);
extern sdfModelTexList_t *sdfTexListDup(sdfModelTexList_t *aList);

void mdlUnRegist(s32 aMajor, s32 aMinor);
void mdlUnRegistResrc(dds3ModelResrc_t *resrc_p);
void mdlFree(mdlObj_t *aHandle);
void mdlFreeMotion(mdlObj_t *aHandle, s32 aGroup);
void mdlUnRegistResrcMotion(dds3ModelResrc_t *aResrc, s32 aGroup);

void mdlRegistMotion(s32 aMajor, s32 aMinor, u32 aFlags, s32 aFirstMotion, s32 aGroup, s32 aPlayGroup,
                     sdfMotionPlayBook_t *aData, sdfMemHandle_t *aHandle);


void mdlMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, s32 aType, f32 aBeforeLeng,
               f32 aSmoothLeng);

sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *aHandle, s32 aGroup);
void mdlSetEffectColor(mdlObj_t *aHandle, u32 aColor);

extern sdfModelTexList_t *sdfTexSetTB(void *aData);
extern sdfModelDef_t *sdfFixupModelBinary(void *aData);
extern sdfMotionPlayBook_t *sdfFixupMotionBinary(void *aData);
void TbMbAbLoadEnd(mdlManager_mdlTbMbAbLoadCb_t *tmaCb);
void mdlFreeBlur(mdlObj_t *aHandle);
extern dds3ModelResrc_t *mdlLoadPrepare(u32 group, u32 id);

typedef struct
{
    u16 group;
    u16 id;
    s32 arg;
    void *owner;
    void (*done)(u32);
    u32 doneArg;
} mdlManager_mdlFileLoadCb_t;

saddr_t mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);
void mdlFreeMotionCtrlr(mdlObj_t *aHandle, sdfMotionCtrlr_t *aCtrlr);
sdfPacDecodeResrc_t *mdlPackSet(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags);
void mdlFileCbFree(mdlManager_mdlFileLoadCb_t *cb);
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

extern mdlFileSlot_t modelFileTable[];
extern mdlFileKindSlot_t modelFileKindTable[];
extern dds3ModelFileIndex_t *mdlHddFileIndex;
extern dds3ModelFileTable_t *mdlHddFileList;
void mdlReleaseHddFileList(void);
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

typedef struct mdlPartTagRecord_t {
    u32 tag;         /* 0x00 */
    u8 state;        /* 0x04 */
    u8 opaque05[3];  /* 0x05 */
} mdlPartTagRecord_t;

struct mdlEffectDescriptor_t;

extern char mdlModelPathPrefix[];
extern char mdlPackedBundleExt[];
extern mdlWorkPrefix_t mdlWorkPathPrefix[];
extern mdlHddFileEntry_t *mdlHddFileCount;
extern u8 sdfHddValid;
extern const char _ctype_[];
extern void sdfDummyFunc(void);
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
void TbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm);
void MbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm);
void AbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm);
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
void mdlGetFilebase(s32 aMajor, s32 aMinor, char *aName, s32 aNameLeng);
s32 mdlPackCheck(sdfPacDecodeResrc_t *resrc);
sdfPacDecodeResrc_t *mdlPackSetOld(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags);
void mdlFileLoadCallback(fileRequest_t *pacHandle, mdlManager_mdlFileLoadCb_t *data);

s32 mdlFileLoad(s32 major, s32 minor, s32 regFlags, const char *filename, void (*callback)(u32),
                u32 parm);

void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);
void mdlFreeFileItem(void *aItem);
void mdlMakeHddFileListDummy(void);
void mdlMakeHddFileList(void);
extern char mdlModelPathPrefix[8];
extern char mdlPackedBundleExt[4];
extern mdlWorkPrefix_t mdlWorkPathPrefix[1];

#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "libsdf.h"
#include "mdlFileStr.h"

struct mdlEffectDescriptor_t;

void mdlLink_Kubi(mdlObj_t *aHandle, void *aModelOt, s32 aNode, f32 aPitch, f32 aYaw);

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

#ifdef VERSION_RAIDOU1
void TbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm)
{
    parm->payload.texList = sdfTexSetTB(fileGetLoadPtr(fileHandle));
    sdfMemFreeDirect(fileGetMemHandle(fileHandle));
    fileHandleFree(fileHandle);
}

#endif
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

#ifdef VERSION_RAIDOU1
void AbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm)
{
    parm->payload.playbookHandle = fileGetMemHandle(fileHandle);
    parm->payload.playbookPtr = sdfFixupMotionBinary(fileGetLoadPtr(fileHandle));
    fileHandleFree(fileHandle);
    TbMbAbLoadEnd(parm);
}

#endif
char *mdlSetFileName(char *buff, const char *fname) {
    *(mdlPathPrefix_t *)buff = *(mdlPathPrefix_t *)mdlModelPathPrefix;
    return strcat(buff, fname);
}

#ifdef VERSION_RAIDOU1
saddr_t mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode) {
    char requestPath[0x80];
    dds3ModelResrc_t *entity;
    dds3ModelFileIndex_t *selection;
    dds3ModelFileTable_t *paths;
    mdlManager_mdlTbMbAbLoadCb_t *request;
    char *extension;
    s32 packedBundle;

    WaitSema(mdlManagerWork);
    entity = mdlSearchResrc(aMajor, aMinor);
    if (entity != NULL) {
        SignalSema(mdlManagerWork);
        return EE_ADDR_S(entity);
    }
    if (mdlCheckLoading(aMajor, aMinor)) {
        SignalSema(mdlManagerWork);
        if (aBlockMode != 0) {
            return EE_ADDR_S(mdlWaitLoadEnd(aMajor, aMinor));
        }
        return -1;
    }
    selection = (dds3ModelFileIndex_t *)modelFileKindTable[aMajor].entries;
    selection += aMinor;
    paths = &((dds3ModelFileTable_t *)modelFileTable[selection->major].entries)[selection->minor];
    packedBundle = 0;
    extension = strrchr(paths->modelFile, '.');
    if (extension != NULL) {
        packedBundle = strcasecmp(extension, mdlPackedBundleExt) == 0;
    }
    mdlRegLoading(aMajor, aMinor);
    SignalSema(mdlManagerWork);
    if (packedBundle != 0) {
        mdlSetFileName(requestPath, paths->modelFile);
        mdlFileLoad(aMajor, aMinor, 0x101, requestPath,
                                  (void (*)(u32))sdfDummyFunc, 0);
    } else {
        request = sdfChipAllocZeroDirect(sizeof(*request));
        request->options = 0x101;
        request->group = aMajor;
        request->id = aMinor;
        if (paths->motionFile != NULL) {
            request->deferred = 1;
        }
        if (paths->texFile != NULL) {
            request->resourceListRequested = 1;
            mdlSetFileName(requestPath, paths->texFile);
            fileLoadEx(requestPath, 0, EE_ADDR(TbLoadCallback), EE_ADDR(request));
        }
        request->itemsRequested = 1;
        mdlSetFileName(requestPath, paths->modelFile);
        fileLoadEx(requestPath, 0, EE_ADDR(MbLoadCallback), EE_ADDR(request));
        if (paths->motionFile != NULL) {
            mdlSetFileName(requestPath, paths->motionFile);
            fileLoadEx(requestPath, 0, EE_ADDR(AbLoadCallback), EE_ADDR(request));
        }
    }
    if (aBlockMode != 0) {
        return EE_ADDR_S(mdlWaitLoadEnd(aMajor, aMinor));
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "model/mdlManager", mdlCallWaitSema);
#endif

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

INCLUDE_ASM(const s32, "model/mdlManager", mdlLink_NoDraw);

/* mdlLink, first turning node aNode's local rotation towards aPitch and aYaw
 * (degrees) by abs(aPitch) / 25, at most one. aNode -1 skips the turn. */
void mdlLink_Kubi(mdlObj_t *aHandle, void *aModelOt, s32 aNode, f32 aPitch, f32 aYaw)
{
    sdfModelRootNode_t *inner;
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

#ifdef VERSION_RAIDOU1
void mdlMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, s32 aType,
                                     f32 aBeforeLeng, f32 aSmoothLeng) {
    sdfMotionCtrlr_t *node;
    s16 slotIndex;

    for (node = aHandle->inner->motion; node != NULL; node = node->next) {
        if (node->userId == aGroup) {
            break;
        }
    }
    if (node == NULL) {
        node = mdlCreateMotion(aHandle, aGroup);
    }
    slotIndex = node->userId2;
    aHandle->slots[slotIndex] = node;
    if (slotIndex == 0) {
        aHandle->first = node;
    }
    sdfMotionChangeBlend(node, aNumber, aType, aBeforeLeng, aSmoothLeng);
    aHandle->current.h.id = aGroup;
    aHandle->current.h.motionIndex = aNumber;
    mdlStopEffect(aHandle, slotIndex);
    mdlGenEffect(aHandle, aNumber, slotIndex);
}
#else
INCLUDE_ASM(const s32, "model/mdlManager", mdlMotion);
#endif

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

#ifdef VERSION_RAIDOU1
void mdlGetFilebase(s32 aMajor, s32 aMinor, char *aName, s32 aNameLeng) {
    dds3ModelFileIndex_t *selection;
    dds3ModelFileTable_t *pathEntry;
    const char *path;
    const char *basename;
    s32 pathLength;
    s32 startIndex;
    s32 endIndex;
    s32 copyLength;

    selection = (dds3ModelFileIndex_t *)modelFileKindTable[aMajor].entries;
    selection += aMinor;
    pathEntry = (dds3ModelFileTable_t *)modelFileTable[selection->major].entries;
    pathEntry += selection->minor;
    path = pathEntry->modelFile;
    startIndex = strlen(path);
    pathLength = startIndex;
    startIndex--;

    while (1) {
        basename = &path[startIndex];
        if (*basename == '/') {
            startIndex++;
            basename = &path[startIndex];
            break;
        }
        if (startIndex == 0) {
            break;
        }
        startIndex--;
    }

    endIndex = startIndex;
    do {
        endIndex++;
        if (endIndex >= pathLength) {
            break;
        }
    } while (path[endIndex] != '.');

    copyLength = endIndex - startIndex;
    aNameLeng--;
    if (aNameLeng < copyLength) {
        copyLength = aNameLeng;
    }
    memcpy(aName, basename, copyLength);
    aName[copyLength] = '\0';
}
#else
INCLUDE_ASM(const s32, "model/mdlManager", mdlGetFilebase);
#endif

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
                count = mdlPibGetEffectCount((pibHeaderChunk_t *)resrc->data);
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
    pibHeaderChunk_t *record;
    pibHeaderChunk_t *slotRecord;
    s32 partCount;
    sdfItemArray_t *partList;
    s32 slotCount;
    s32 objectSize;
    s32 remainingParts;
    s32 payloadValue;

    if (mdlPackCheck(resrc) == 0) {
        return mdlPackSetOld(resrc, major, minor, regFlags);
    }
    record = (pibHeaderChunk_t *)resrc->data;
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
                /* Retained metadata stores, as SMT3 1DE078/1DE07C
                 * (an otherwise unused local record). */
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
        slotRecord = (pibHeaderChunk_t *)(resrc->data - (u32)&((pibHeaderChunk_t *)0)->tail);
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

void mdlFileLoadCallback(fileRequest_t *pacHandle, mdlManager_mdlFileLoadCb_t *data)
{
    data->owner = pacHandle;
    mdlPackSet(pacHandle->pac.resrcHead, data->group, data->id, data->arg);
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
    fileRequest_t *request;

    job = sdfChipAllocZeroDirect(sizeof(mdlManager_mdlFileLoadCb_t));
    job->group = major;
    job->id = minor;
    job->arg = regFlags;
    job->doneArg = parm;
    job->done = callback;
    request = pacLoadEx(filename, 0, 0, EE_ADDR(mdlFileLoadCallback), EE_ADDR(job));
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

#ifdef VERSION_RAIDOU1
void mdlReleaseHddFileList(void)
{
    dds3ModelFileTable_t *entries;
    s32 count;
    s32 i;

    count = modelFileTable[5].count;
    if (count > 0)
    {
        entries = modelFileTable[5].entries;
        i = 0;
        do
        {
            sdfChipFreeDirect(entries[i].modelFile);
            sdfChipFreeDirect(entries[i].texFile);
            sdfChipFreeDirect(entries[i].motionFile);
            i++;
        } while (i < count);
    }
    modelFileKindTable[5].entries = NULL;
    modelFileKindTable[5].count = 0;
    modelFileTable[5].entries = NULL;
    modelFileTable[5].count = 0;
    sdfMemFreeHandleDirect(&mdlHddFileIndexHandle);
    sdfMemFreeHandleDirect(&mdlHddFileListHandle);
}
#else
INCLUDE_ASM(const s32, "model/mdlManager", mdlReleaseHddFileList);

INCLUDE_ASM(const s32, "model/mdlManager", mdlMakeHddFileListDummy);
#endif

#ifdef VERSION_RAIDOU1
void mdlMakeHddFileListDummy(void)
{
    char *name;
    char *src;
    s32 i;

    mdlReleaseHddFileList();
    mdlHddFileIndexHandle = sdfMemAllocDirect(sizeof(dds3ModelFileIndex_t));
    mdlHddFileIndex = sdfMemLock(mdlHddFileIndexHandle);
    mdlHddFileIndex->major = 5;
    mdlHddFileIndex->minor = 0;
    mdlHddFileIndex->scale = 0x1000;
    mdlHddFileIndex->radius = 1000;
    mdlHddFileListHandle = sdfMemAllocDirect(sizeof(dds3ModelFileTable_t));
    mdlHddFileList = sdfMemLock(mdlHddFileListHandle);
    for (i = 0; i != 3; i++)
    {
        switch (i)
        {
        case 0:
            src = modelFileTable[0].entries->modelFile;
            break;
        case 1:
            src = modelFileTable[0].entries->texFile;
            break;
        default:
            src = modelFileTable[0].entries->motionFile;
            break;
        }
        if (src == NULL)
        {
            continue;
        }
        name = sdfChipAllocDirect(strlen(src) + 1);
        strcpy(name, src);
        switch (i)
        {
        case 0:
            mdlHddFileList->modelFile = name;
            break;
        case 1:
            mdlHddFileList->texFile = name;
            break;
        case 2:
            mdlHddFileList->motionFile = name;
            break;
        }
    }
    modelFileKindTable[5].entries = mdlHddFileIndex;
    modelFileKindTable[5].count = 1;
    modelFileTable[5].entries = mdlHddFileList;
    modelFileTable[5].count = 1;
}

void mdlMakeHddFileList(void) {
    mdlDirectoryEntry_t directory;
    s32 index;
    s32 count;
    s32 handle;
    s32 length;
    s32 end;
    mdlHddFileEntry_t *next;
    mdlHddFileEntry_t *file;
    mdlHddFileEntry_t **link;

    if (sdfHddValid && (handle = sceDopen("pfs0:/model/work")) >= 0) {
        mdlReleaseHddFileList();
        while ((length = sceDread(handle, &directory)) > 0) {
            if ((directory.mode & 0xF000) != 0x1000 && length >= 4 && length < 49) {
                s32 kind = -1;

                if (directory.name[length - 3] == '.' &&
                    (s8)FileUpper(directory.name[length - 1]) == 'B') {
                    s32 ext = directory.name[length - 2];

                    if (MDL_CHAR_IS_LOWER(ext)) {
                        ext -= 'a' - 'A';
                    }
                    switch (ext) {
                    case 'A':
                        kind = 2;
                        break;
                    case 'T':
                        kind = 1;
                        break;
                    case 'M':
                    case 'P':
                        kind = 0;
                        break;
                    }
                    if (kind >= 0) {
                        file = mdlHddFileCount;
                        while (file != NULL && memcmp(file->name, directory.name, length - 2) != 0) {
                            file = file->next;
                        }
                        if (file == NULL) {
                            file = sdfChipAllocDirect(sizeof(*file));
                            memset(file, 0, sizeof(*file));
                            memcpy(file->name, directory.name, length - 2);
                            file->name[length - 2] = 0;
                            file->next = mdlHddFileCount;
                            mdlHddFileCount = file;
                        }
                        file->available[kind] = 1;
                        file->extension[kind][0] = directory.name[length - 2];
                        file->extension[kind][1] = directory.name[length - 1];
                    }
                }
            }
        }
        count = 0;
        sceDclose(handle);
        link = &mdlHddFileCount;
        next = mdlHddFileCount;
        while (next != NULL) {
            file = next;
            next = file->next;
            if (!file->available[0]) {
                *link = next;
                mdlFreeFileItem(file);
            } else {
                if (FileUpper(file->extension[0][0]) == 'P') {
                    file->available[1] = 0;
                    file->available[2] = 0;
                }
                link = &file->next;
                count++;
            }
        }
        if (count != 0) {
            index = 0;
            mdlHddFileIndexHandle = sdfMemAllocDirect(count * sizeof(dds3ModelFileIndex_t));
            mdlHddFileIndex = sdfMemLock(mdlHddFileIndexHandle);
            mdlHddFileListHandle = sdfMemAllocDirect(count * sizeof(dds3ModelFileTable_t));
            mdlHddFileList = sdfMemLock(mdlHddFileListHandle);
            memset(mdlHddFileList, 0, count * sizeof(dds3ModelFileTable_t));
            file = mdlHddFileCount;
            while (file != NULL) {
                const char *name = file->name;
                s32 kind;

                mdlHddFileIndex[index].major = 5;
                mdlHddFileIndex[index].minor = index;
                mdlHddFileIndex[index].scale = 0x1000;
                mdlHddFileIndex[index].radius = 1000;
                length = strlen(name);
                end = length + 5;
                for (kind = 0; kind != 3; kind++) {
                    if (file->available[kind]) {
                        char *path = sdfChipAllocDirect(length + 8);

                        *(mdlWorkPrefix_t *)path = mdlWorkPathPrefix[0];
                        memcpy(path + 5, name, length);
                        path[end] = file->extension[kind][0];
                        path[end + 1] = file->extension[kind][1];
                        path[end + 2] = 0;
                        switch (kind) {
                        case 0:
                            mdlHddFileList[index].modelFile = path;
                            break;
                        case 1:
                            mdlHddFileList[index].texFile = path;
                            break;
                        case 2:
                            mdlHddFileList[index].motionFile = path;
                            break;
                        }
                    }
                }
                file = file->next;
                index++;
            }
            modelFileKindTable[5].entries = mdlHddFileIndex;
            modelFileKindTable[5].count = count;
            modelFileTable[5].entries = mdlHddFileList;
            modelFileTable[5].count = count;
            return;
        }
    }
    mdlMakeHddFileListDummy();
}
#else
INCLUDE_ASM(const s32, "model/mdlManager", mdlMakeHddFileList);
#endif
char mdlModelPathPrefix[8] = "/model/";

#ifdef VERSION_RAIDOU1
char mdlPackedBundleExt[4] = ".PB";
#else
INCLUDE_SDATA(const s32, "model/mdlManager", mdlPackedBundleExt);
#endif

#ifdef VERSION_RAIDOU1
sdfMemHandle_t *mdlHddFileIndexHandle = NULL;
#else
INCLUDE_SDATA(const s32, "model/mdlManager", mdlHddFileIndexHandle);
#endif

#ifdef VERSION_RAIDOU1
sdfMemHandle_t *mdlHddFileListHandle = NULL;
#else
INCLUDE_SDATA(const s32, "model/mdlManager", mdlHddFileListHandle);
#endif

#ifdef VERSION_RAIDOU1
mdlWorkPrefix_t mdlWorkPathPrefix[1] = { { "work/" } };
#else
INCLUDE_SDATA(const s32, "model/mdlManager", mdlWorkPathPrefix);
#endif

#ifdef VERSION_RAIDOU1
dds3ModelFileTable_t modelFileTableEntry[34] = {
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHuman000Pb2, NULL },
    { NULL, modelStrBattleHumanNakedPb, NULL },
    { NULL, modelStrBattleHuman000Pb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleHumanPlayerPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemTakara2Pb, NULL },
    { NULL, modelStrBattleItemTakara3Pb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleItemNpcBoxPb, NULL },
    { NULL, modelStrBattleArms000Katana02Pb, NULL },
    { NULL, modelStrBattleArms000KatanaPb, NULL },
};

dds3ModelFileTable_t modelFileTableEntry2[352] = {
    { NULL, modelStrDevil000Pb, NULL },
    { NULL, modelStrDevil001Pb, NULL },
    { NULL, modelStrDevil002Pb, NULL },
    { NULL, modelStrDevil003Pb, NULL },
    { NULL, modelStrDevil004Pb, NULL },
    { NULL, modelStrDevil005Pb, NULL },
    { NULL, modelStrDevil006Pb, NULL },
    { NULL, modelStrDevil007Pb, NULL },
    { NULL, modelStrDevil008Pb, NULL },
    { NULL, modelStrDevil009Pb, NULL },
    { NULL, modelStrDevil00APb, NULL },
    { NULL, modelStrDevil00BPb, NULL },
    { NULL, modelStrDevil00CPb, NULL },
    { NULL, modelStrDevil00DPb, NULL },
    { NULL, modelStrDevil00EPb, NULL },
    { NULL, modelStrDevil00FPb, NULL },
    { NULL, modelStrDevil010Pb, NULL },
    { NULL, modelStrDevil011Pb, NULL },
    { NULL, modelStrDevil012Pb, NULL },
    { NULL, modelStrDevil013Pb, NULL },
    { NULL, modelStrDevil014Pb, NULL },
    { NULL, modelStrDevil015Pb, NULL },
    { NULL, modelStrDevil016Pb, NULL },
    { NULL, modelStrDevil017Pb, NULL },
    { NULL, modelStrDevil018Pb, NULL },
    { NULL, modelStrDevil019Pb, NULL },
    { NULL, modelStrDevil01APb, NULL },
    { NULL, modelStrDevil01BPb, NULL },
    { NULL, modelStrDevil01CPb, NULL },
    { NULL, modelStrDevil01DPb, NULL },
    { NULL, modelStrDevil01EPb, NULL },
    { NULL, modelStrDevil01FPb, NULL },
    { NULL, modelStrDevil020Pb, NULL },
    { NULL, modelStrDevil021Pb, NULL },
    { NULL, modelStrDevil022Pb, NULL },
    { NULL, modelStrDevil023Pb, NULL },
    { NULL, modelStrDevil024Pb, NULL },
    { NULL, modelStrDevil025Pb, NULL },
    { NULL, modelStrDevil026Pb, NULL },
    { NULL, modelStrDevil027Pb, NULL },
    { NULL, modelStrDevil028Pb, NULL },
    { NULL, modelStrDevil029Pb, NULL },
    { NULL, modelStrDevil02APb, NULL },
    { NULL, modelStrDevil02BPb, NULL },
    { NULL, modelStrDevil02CPb, NULL },
    { NULL, modelStrDevil02DPb, NULL },
    { NULL, modelStrDevil02EPb, NULL },
    { NULL, modelStrDevil02FPb, NULL },
    { NULL, modelStrDevil030Pb, NULL },
    { NULL, modelStrDevil031Pb, NULL },
    { NULL, modelStrDevil032Pb, NULL },
    { NULL, modelStrDevil033Pb, NULL },
    { NULL, modelStrDevil034Pb, NULL },
    { NULL, modelStrDevil035Pb, NULL },
    { NULL, modelStrDevil036Pb, NULL },
    { NULL, modelStrDevil037Pb, NULL },
    { NULL, modelStrDevil038Pb, NULL },
    { NULL, modelStrDevil039Pb, NULL },
    { NULL, modelStrDevil03APb, NULL },
    { NULL, modelStrDevil03BPb, NULL },
    { NULL, modelStrDevil03CPb, NULL },
    { NULL, modelStrDevil03DPb, NULL },
    { NULL, modelStrDevil03EPb, NULL },
    { NULL, modelStrDevil03FPb, NULL },
    { NULL, modelStrDevil040Pb, NULL },
    { NULL, modelStrDevil041Pb, NULL },
    { NULL, modelStrDevil042Pb, NULL },
    { NULL, modelStrDevil043Pb, NULL },
    { NULL, modelStrDevil044Pb, NULL },
    { NULL, modelStrDevil045Pb, NULL },
    { NULL, modelStrDevil046Pb, NULL },
    { NULL, modelStrDevil047Pb, NULL },
    { NULL, modelStrDevil021Pb, NULL },
    { NULL, modelStrDevil021Pb, NULL },
    { NULL, modelStrDevil021Pb, NULL },
    { NULL, modelStrDevil021Pb, NULL },
    { NULL, modelStrDevil04CPb, NULL },
    { NULL, modelStrDevil04DPb, NULL },
    { NULL, modelStrDevil04EPb, NULL },
    { NULL, modelStrDevil04FPb, NULL },
    { NULL, modelStrDevil050Pb, NULL },
    { NULL, modelStrDevil001Pb, NULL },
    { NULL, modelStrDevil002Pb, NULL },
    { NULL, modelStrDevil003Pb, NULL },
    { NULL, modelStrDevil004Pb, NULL },
    { NULL, modelStrDevil005Pb, NULL },
    { NULL, modelStrDevil006Pb, NULL },
    { NULL, modelStrDevil007Pb, NULL },
    { NULL, modelStrDevil008Pb, NULL },
    { NULL, modelStrDevil009Pb, NULL },
    { NULL, modelStrDevil00APb, NULL },
    { NULL, modelStrDevil00BPb, NULL },
    { NULL, modelStrDevil00CPb, NULL },
    { NULL, modelStrDevil00DPb, NULL },
    { NULL, modelStrDevil00EPb, NULL },
    { NULL, modelStrDevil00FPb, NULL },
    { NULL, modelStrDevil010Pb, NULL },
    { NULL, modelStrDevil011Pb, NULL },
    { NULL, modelStrDevil012Pb, NULL },
    { NULL, modelStrDevil013Pb, NULL },
    { NULL, modelStrDevil014Pb, NULL },
    { NULL, modelStrDevil015Pb, NULL },
    { NULL, modelStrDevil016Pb, NULL },
    { NULL, modelStrDevil017Pb, NULL },
    { NULL, modelStrDevil018Pb, NULL },
    { NULL, modelStrDevil019Pb, NULL },
    { NULL, modelStrDevil01APb, NULL },
    { NULL, modelStrDevil01BPb, NULL },
    { NULL, modelStrDevil01CPb, NULL },
    { NULL, modelStrDevil01DPb, NULL },
    { NULL, modelStrDevil01EPb, NULL },
    { NULL, modelStrDevil01FPb, NULL },
    { NULL, modelStrDevil020Pb, NULL },
    { NULL, modelStrDevil021Pb, NULL },
    { NULL, modelStrDevil022Pb, NULL },
    { NULL, modelStrDevil023Pb, NULL },
    { NULL, modelStrDevil024Pb, NULL },
    { NULL, modelStrDevil025Pb, NULL },
    { NULL, modelStrDevil026Pb, NULL },
    { NULL, modelStrDevil027Pb, NULL },
    { NULL, modelStrDevil028Pb, NULL },
    { NULL, modelStrDevil029Pb, NULL },
    { NULL, modelStrDevil02APb, NULL },
    { NULL, modelStrDevil02BPb, NULL },
    { NULL, modelStrDevil02CPb, NULL },
    { NULL, modelStrDevil02DPb, NULL },
    { NULL, modelStrDevil02EPb, NULL },
    { NULL, modelStrDevil02FPb, NULL },
    { NULL, modelStrDevil030Pb, NULL },
    { NULL, modelStrDevil031Pb, NULL },
    { NULL, modelStrDevil032Pb, NULL },
    { NULL, modelStrDevil033Pb, NULL },
    { NULL, modelStrDevil034Pb, NULL },
    { NULL, modelStrDevil035Pb, NULL },
    { NULL, modelStrDevil036Pb, NULL },
    { NULL, modelStrDevil037Pb, NULL },
    { NULL, modelStrDevil038Pb, NULL },
    { NULL, modelStrDevil039Pb, NULL },
    { NULL, modelStrDevil03APb, NULL },
    { NULL, modelStrDevil03BPb, NULL },
    { NULL, modelStrDevil03CPb, NULL },
    { NULL, modelStrDevil03DPb, NULL },
    { NULL, modelStrDevil03EPb, NULL },
    { NULL, modelStrDevil03FPb, NULL },
    { NULL, modelStrDevil040Pb, NULL },
    { NULL, modelStrDevil041Pb, NULL },
    { NULL, modelStrDevil092Pb, NULL },
    { NULL, modelStrDevil093Pb, NULL },
    { NULL, modelStrDevil094Pb, NULL },
    { NULL, modelStrDevil095Pb, NULL },
    { NULL, modelStrDevil043Pb, NULL },
    { NULL, modelStrDevil097Pb, NULL },
    { NULL, modelStrDevil098Pb, NULL },
    { NULL, modelStrDevil042Pb, NULL },
    { NULL, modelStrDevil045Pb, NULL },
    { NULL, modelStrDevil111Pb, NULL },
    { NULL, modelStrDevil044Pb, NULL },
    { NULL, modelStrDevil046Pb, NULL },
    { NULL, modelStrDevil047Pb, NULL },
    { NULL, modelStrDevil09FPb, NULL },
    { NULL, modelStrDevil092Pb, NULL },
    { NULL, modelStrDevil092Pb, NULL },
    { NULL, modelStrDevil00DPb, NULL },
    { NULL, modelStrDevil036Pb, NULL },
    { NULL, modelStrDevil00CPb, NULL },
    { NULL, modelStrDevil031Pb, NULL },
    { NULL, modelStrDevil027Pb, NULL },
    { NULL, modelStrDevil094Pb, NULL },
    { NULL, modelStrDevil0A8Pb, NULL },
    { NULL, modelStrDevil02CPb, NULL },
    { NULL, modelStrDevil02BPb, NULL },
    { NULL, modelStrDevil023Pb, NULL },
    { NULL, modelStrDevil095Pb, NULL },
    { NULL, modelStrDevil098Pb, NULL },
    { NULL, modelStrDevil017Pb, NULL },
    { NULL, modelStrDevil013Pb, NULL },
    { NULL, modelStrDevil012Pb, NULL },
    { NULL, modelStrDevil00BPb, NULL },
    { NULL, modelStrDevil030Pb, NULL },
    { NULL, modelStrDevil001Pb, NULL },
    { NULL, modelStrDevil00EPb, NULL },
    { NULL, modelStrDevil010Pb, NULL },
    { NULL, modelStrDevil0B6Pb, NULL },
    { NULL, modelStrDevil014Pb, NULL },
    { NULL, modelStrDevil036Pb, NULL },
    { NULL, modelStrDevil02BPb, NULL },
    { NULL, modelStrDevil094Pb, NULL },
    { NULL, modelStrDevil015Pb, NULL },
    { NULL, modelStrDevil0C1Pb, NULL },
    { NULL, modelStrDevil0BdPb, NULL },
    { NULL, modelStrDevil0BePb, NULL },
    { NULL, modelStrDevil0BfPb, NULL },
    { NULL, modelStrDevil0BcPb, NULL },
    { NULL, modelStrDevil0C1Pb, NULL },
    { NULL, modelStrDevil0C2Pb, NULL },
    { NULL, modelStrDevil0BePb, NULL },
    { NULL, modelStrDevil0BfPb, NULL },
    { NULL, modelStrDevil0C1Pb, NULL },
    { NULL, modelStrDevil0BdPb, NULL },
    { NULL, modelStrDevil0BdPb, NULL },
    { NULL, modelStrDevil0BdPb, NULL },
    { NULL, modelStrDevil0BePb, NULL },
    { NULL, modelStrDevil0BfPb, NULL },
    { NULL, modelStrDevil0C1Pb, NULL },
    { NULL, modelStrDevil0BcPb, NULL },
    { NULL, modelStrDevil0C2Pb, NULL },
    { NULL, modelStrDevil0BdPb, NULL },
    { NULL, modelStrDevil0BePb, NULL },
    { NULL, modelStrDevil0BfPb, NULL },
    { NULL, modelStrDevil0C1Pb, NULL },
    { NULL, modelStrDevil0BcPb, NULL },
    { NULL, modelStrDevil0BdPb, NULL },
    { NULL, modelStrDevil0BdPb, NULL },
    { NULL, modelStrDevil033Pb, NULL },
    { NULL, modelStrDevil02EPb, NULL },
    { NULL, modelStrDevil004Pb, NULL },
    { NULL, modelStrDevil004Pb, NULL },
    { NULL, modelStrDevil095Pb, NULL },
    { NULL, modelStrDevil095Pb, NULL },
    { NULL, modelStrDevil098Pb, NULL },
    { NULL, modelStrDevil009Pb, NULL },
    { NULL, modelStrDevil03CPb, NULL },
    { NULL, modelStrDevil026Pb, NULL },
    { NULL, modelStrDevil015Pb, NULL },
    { NULL, modelStrDevil033Pb, NULL },
    { NULL, modelStrDevil043Pb, NULL },
    { NULL, modelStrDevil025Pb, NULL },
    { NULL, modelStrDevil029Pb, NULL },
    { NULL, modelStrDevil031Pb, NULL },
    { NULL, modelStrDevil01FPb, NULL },
    { NULL, modelStrDevil009Pb, NULL },
    { NULL, modelStrDevil00DPb, NULL },
    { NULL, modelStrDevil039Pb, NULL },
    { NULL, modelStrDevil092Pb, NULL },
    { NULL, modelStrDevil018Pb, NULL },
    { NULL, modelStrDevil010Pb, NULL },
    { NULL, modelStrDevil038Pb, NULL },
    { NULL, modelStrDevil092Pb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevil00EPb, NULL },
    { NULL, modelStrDevil038Pb, NULL },
    { NULL, modelStrDevil02APb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevilDummyPb, NULL },
    { NULL, modelStrDevil100Pb, NULL },
    { NULL, modelStrDevil101Pb, NULL },
    { NULL, modelStrDevil102Pb, NULL },
    { NULL, modelStrDevil103Pb, NULL },
    { NULL, modelStrDevil104Pb, NULL },
    { NULL, modelStrDevil105Pb, NULL },
    { NULL, modelStrDevil106Pb, NULL },
    { NULL, modelStrDevil107Pb, NULL },
    { NULL, modelStrDevil039Pb, NULL },
    { NULL, modelStrDevil109Pb, NULL },
    { NULL, modelStrDevil10APb, NULL },
    { NULL, modelStrDevil10BPb, NULL },
    { NULL, modelStrDevil10CPb, NULL },
    { NULL, modelStrDevil10DPb, NULL },
    { NULL, modelStrDevil10EPb, NULL },
    { NULL, modelStrDevil10FPb, NULL },
    { NULL, modelStrDevil110Pb, NULL },
    { NULL, modelStrDevil111Pb, NULL },
    { NULL, modelStrDevil111Pb, NULL },
    { NULL, modelStrDevil113Pb, NULL },
    { NULL, modelStrDevil114Pb, NULL },
    { NULL, modelStrDevil115Pb, NULL },
    { NULL, modelStrDevil113Pb, NULL },
    { NULL, modelStrDevil114Pb, NULL },
    { NULL, modelStrDevil113Pb, NULL },
    { NULL, modelStrDevil114Pb, NULL },
    { NULL, modelStrDevil113Pb, NULL },
    { NULL, modelStrDevil114Pb, NULL },
    { NULL, modelStrDevil115Pb, NULL },
    { NULL, modelStrDevil11DPb, NULL },
    { NULL, modelStrDevil11EPb, NULL },
    { NULL, modelStrDevil001Pb, NULL },
    { NULL, modelStrDevil120Pb, NULL },
    { NULL, modelStrDevil121Pb, NULL },
    { NULL, modelStrDevil122Pb, NULL },
    { NULL, modelStrDevil123Pb, NULL },
    { NULL, modelStrDevil124Pb, NULL },
    { NULL, modelStrDevil016Pb, NULL },
    { NULL, modelStrDevil126Pb, NULL },
    { NULL, modelStrDevil127Pb, NULL },
    { NULL, modelStrDevil128Pb, NULL },
    { NULL, modelStrDevil122Pb, NULL },
    { NULL, modelStrDevil027Pb, NULL },
    { NULL, modelStrDevil111Pb, NULL },
    { NULL, modelStrDevil106Pb, NULL },
    { NULL, modelStrDevil105Pb, NULL },
    { NULL, modelStrDevil107Pb, NULL },
    { NULL, modelStrDevil02BPb, NULL },
    { NULL, modelStrDevil10DPb, NULL },
    { NULL, modelStrDevil10CPb, NULL },
    { NULL, modelStrDevil121Pb, NULL },
    { NULL, modelStrDevil11EPb, NULL },
    { NULL, modelStrDevil11DPb, NULL },
    { NULL, modelStrDevil120Pb, NULL },
    { NULL, modelStrDevil10BPb, NULL },
    { NULL, modelStrDevil115Pb, NULL },
    { NULL, modelStrDevil138Pb, NULL },
    { NULL, modelStrDevil138Pb, NULL },
    { NULL, modelStrDevil138Pb, NULL },
    { NULL, modelStrDevil13BPb, NULL },
    { NULL, modelStrDevil13CPb, NULL },
    { NULL, modelStrDevil13DPb, NULL },
    { NULL, modelStrDevil13EPb, NULL },
    { NULL, modelStrDevil13FPb, NULL },
    { NULL, modelStrDevil140Pb, NULL },
    { NULL, modelStrDevil141Pb, NULL },
    { NULL, modelStrDevil142Pb, NULL },
    { NULL, modelStrDevil036Pb, NULL },
    { NULL, modelStrDevil02EPb, NULL },
    { NULL, modelStrDevil039Pb, NULL },
    { NULL, modelStrDevil146Pb, NULL },
    { NULL, modelStrDevil036Pb, NULL },
    { NULL, modelStrDevil032Pb, NULL },
    { NULL, modelStrDevil00EPb, NULL },
    { NULL, modelStrDevil02DPb, NULL },
    { NULL, modelStrDevil04DPb, NULL },
    { NULL, modelStrDevil00BPb, NULL },
    { NULL, modelStrDevil039Pb, NULL },
    { NULL, modelStrDevil02BPb, NULL },
    { NULL, modelStrDevil003Pb, NULL },
    { NULL, modelStrDevil02BPb, NULL },
    { NULL, modelStrDevil02DPb, NULL },
    { NULL, modelStrDevil030Pb, NULL },
    { NULL, modelStrDevil054Pb, NULL },
    { NULL, modelStrDevil036Pb, NULL },
    { NULL, modelStrDevil062Pb, NULL },
    { NULL, modelStrDevil062Pb, NULL },
    { NULL, modelStrDevil062Pb, NULL },
    { NULL, modelStrDevil158Pb, NULL },
    { NULL, modelStrDevil159Pb, NULL },
    { NULL, modelStrDevil15APb, NULL },
    { NULL, modelStrDevil15BPb, NULL },
    { NULL, modelStrDevil15CPb, NULL },
    { NULL, modelStrDevil15DPb, NULL },
    { NULL, modelStrDevil15EPb, NULL },
    { NULL, modelStrDevil15FPb, NULL },
};

dds3ModelFileTable_t modelFileTableEntry3[80] = {
    { NULL, modelStrEvent000Pb, NULL },
    { NULL, modelStrEventRaidouInPb, NULL },
    { NULL, modelStrEventNarumiInPb, NULL },
    { NULL, modelStrEventKayaPb, NULL },
    { NULL, modelStrEventKayaShortPb, NULL },
    { NULL, modelStrEventMunakataPb2, NULL },
    { NULL, modelStrEventRausuPb, NULL },
    { NULL, modelStrEventKityouPb, NULL },
    { NULL, modelStrEventYataPb, NULL },
    { NULL, modelStrEventNakamuraPb, NULL },
    { NULL, modelStrEventKiyoshiPb, NULL },
    { NULL, modelStrEventSatakeOutPb, NULL },
    { NULL, modelStrEventVictorPb, NULL },
    { NULL, modelStrEventKonnouManPb, NULL },
    { NULL, modelStrEventMilkhallManPb, NULL },
    { NULL, modelStrEventRaidouOutPb, NULL },
    { NULL, modelStrEvent010Pb, NULL },
    { NULL, modelStrEventNarumiOutPb, NULL },
    { NULL, modelStrEventKeijiPb, NULL },
    { NULL, modelStrEventRocketPapaPb, NULL },
    { NULL, modelStrEventRaioduKInPb, NULL },
    { NULL, modelStrEventRaioduKOutPb, NULL },
    { NULL, modelStrEventAibouKakoPb, NULL },
    { NULL, modelStrEventNarumiTelPb, NULL },
    { NULL, modelStrEventRinPb, NULL },
    { NULL, modelStrEventBrotherPb, NULL },
    { NULL, modelStrEventSatakeInPb, NULL },
    { NULL, modelStrEventKayaMiraiPb, NULL },
    { NULL, modelStrEventSadakitiPb, NULL },
    { NULL, modelStrEventSadakiti2Pb, NULL },
    { NULL, modelStrEventMunakataPb, NULL },
    { NULL, modelStrEventSisterPb, NULL },
    { NULL, modelStrEventOkamiPb, NULL },
    { NULL, modelStrEventRausuHeadPb, NULL },
    { NULL, modelStrEventYomiPb, NULL },
    { NULL, modelStrEventBattleRausuPb, NULL },
    { NULL, modelStrEventRedMantPb, NULL },
    { NULL, modelStrEventRausuBlackPb, NULL },
    { NULL, modelStrEvent036KaiwaPb, NULL },
    { NULL, modelStrEvent007KaiwaPb, NULL },
    { NULL, modelStrEvent012KaiwaPb, NULL },
    { NULL, modelStrEvent017KaiwaPb, NULL },
    { NULL, modelStrEvent030KaiwaPb, NULL },
    { NULL, modelStrEvent007BsPb, NULL },
    { NULL, modelStrEvent02CPb, NULL },
    { NULL, modelStrEvent02DPb, NULL },
    { NULL, modelStrEvent02EPb, NULL },
    { NULL, modelStrEvent02FPb, NULL },
    { NULL, modelStrEvent030Pb, NULL },
    { NULL, modelStrEvent031Pb, NULL },
    { NULL, modelStrEvent032Pb, NULL },
    { NULL, modelStrEvent033Pb, NULL },
    { NULL, modelStrEvent034Pb, NULL },
    { NULL, modelStrEvent035Pb, NULL },
    { NULL, modelStrEvent036Pb, NULL },
    { NULL, modelStrEvent037Pb, NULL },
    { NULL, modelStrEvent038Pb, NULL },
    { NULL, modelStrEvent039Pb, NULL },
    { NULL, modelStrEvent03APb, NULL },
    { NULL, modelStrEvent03BPb, NULL },
    { NULL, modelStrEvent03CPb, NULL },
    { NULL, modelStrEvent03DPb, NULL },
    { NULL, modelStrEvent03EPb, NULL },
    { NULL, modelStrEvent03FPb, NULL },
    { NULL, modelStrEvent040Pb, NULL },
    { NULL, modelStrEvent041Pb, NULL },
    { NULL, modelStrEvent042Pb, NULL },
    { NULL, modelStrEvent043Pb, NULL },
    { NULL, modelStrEvent044Pb, NULL },
    { NULL, modelStrEvent045Pb, NULL },
    { NULL, modelStrEvent046Pb, NULL },
    { NULL, modelStrEvent047Pb, NULL },
    { NULL, modelStrEvent048Pb, NULL },
    { NULL, modelStrEvent049Pb, NULL },
    { NULL, modelStrEvent04APb, NULL },
    { NULL, modelStrEvent04BPb, NULL },
    { NULL, modelStrEvent04CPb, NULL },
    { NULL, modelStrEvent04DPb, NULL },
    { NULL, modelStrEvent04EPb, NULL },
    { NULL, modelStrEventKatanaPb, NULL },
};

dds3ModelFileTable_t modelFileTableEntry4[80] = {
    { NULL, modelStrField000Pb2, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldSky01Pb, NULL },
    { NULL, modelStrFieldSky02Pb, NULL },
    { NULL, modelStrFieldCat01Pb, NULL },
    { NULL, modelStrFieldTrainPb, NULL },
    { NULL, modelStrField000Pb, NULL },
    { NULL, modelStrFieldNakedPb2, NULL },
    { NULL, modelStrFieldNakedPb, NULL },
    { NULL, modelStrFieldNpcPb, NULL },
    { NULL, modelStrField00CPb, NULL },
    { NULL, modelStrFieldExitPb, NULL },
    { NULL, modelStrFieldMapShipPb, NULL },
    { NULL, modelStrFieldFloor01Pb, NULL },
    { NULL, modelStrFieldBg09GatePb, NULL },
    { NULL, modelStrFieldBg13WarpPb, NULL },
    { NULL, modelStrFieldTakara01Pb, NULL },
    { NULL, modelStrFieldTakara02Pb, NULL },
    { NULL, modelStrFieldKPointPb, NULL },
    { NULL, modelStrFieldSavePb, NULL },
    { NULL, modelStrFieldSavePb, NULL },
    { NULL, modelStrFieldSavePb, NULL },
    { NULL, modelStrFieldNakisawamePb, NULL },
    { NULL, modelStrFieldHealPb, NULL },
    { NULL, modelStrFieldHealPb, NULL },
    { NULL, modelStrFieldFldSlimePb, NULL },
    { NULL, modelStrFieldKanameFirePb, NULL },
    { NULL, modelStrFieldLineSmokPb, NULL },
    { NULL, modelStrFieldBg09SimbolPb, NULL },
    { NULL, modelStrFieldBg24IcePb, NULL },
    { NULL, modelStrFieldKPoint25Pb, NULL },
    { NULL, modelStrFieldOomagatuPb, NULL },
    { NULL, modelStrFieldBg01BrigePb, NULL },
    { NULL, modelStrFieldBg022Pb, NULL },
    { NULL, modelStrFieldBg082Pb, NULL },
    { NULL, modelStrFieldBg13LastFloorPb, NULL },
    { NULL, modelStrFieldBg13LastWaterPb, NULL },
    { NULL, modelStrFieldBg13GatePb, NULL },
    { NULL, modelStrFieldBg13PlasmaPb2, NULL },
    { NULL, modelStrFieldBg13PlasmaPb, NULL },
    { NULL, modelStrFieldBg13201Pb, NULL },
    { NULL, modelStrFieldBg13202Pb, NULL },
    { NULL, modelStrFieldBg04SmokePb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldMilkhallTablePb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldJigenPb, NULL },
    { NULL, modelStrFieldShokiPb, NULL },
    { NULL, modelStrFieldFirePb, NULL },
    { NULL, modelStrFieldIcePb, NULL },
    { NULL, modelStrFieldThuderPb, NULL },
    { NULL, modelStrFieldKazePb, NULL },
    { NULL, modelStrFieldGunPb, NULL },
    { NULL, modelStrFieldPowerPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
    { NULL, modelStrFieldWpcPb, NULL },
};

dds3ModelFileTable_t modelFileTableEntry5[224] = {
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcNalmi01Pb, NULL },
    { NULL, modelStrNpcKaya01Pb, NULL },
    { NULL, modelStrNpcKaya02Pb, NULL },
    { NULL, modelStrNpcMuna01Pb, NULL },
    { NULL, modelStrNpcRasu01Pb, NULL },
    { NULL, modelStrNpcRasu02Pb, NULL },
    { NULL, modelStrNpcKityou01Pb, NULL },
    { NULL, modelStrNpcKarasu01Pb, NULL },
    { NULL, modelStrNpcKiyoshi01Pb, NULL },
    { NULL, modelStrNpcKiyoshi02Pb, NULL },
    { NULL, modelStrNpcNaka01Pb, NULL },
    { NULL, modelStrNpcSatake01Pb, NULL },
    { NULL, modelStrNpcSatake02Pb, NULL },
    { NULL, modelStrNpcRocket01Pb, NULL },
    { NULL, modelStrNpcRin01Pb, NULL },
    { NULL, modelStrNpcIida01Pb, NULL },
    { NULL, modelStrNpcManto01Pb, NULL },
    { NULL, modelStrNpcSizu01Pb, NULL },
    { NULL, modelStrNpcOkami01Pb, NULL },
    { NULL, modelStrNpcKin01Pb, NULL },
    { NULL, modelStrNpcSada01Pb, NULL },
    { NULL, modelStrNpcSada02Pb, NULL },
    { NULL, modelStrNpcRaidou01Pb, NULL },
    { NULL, modelStrNpcKobun01Pb, NULL },
    { NULL, modelStrNpcKobun02Pb, NULL },
    { NULL, modelStrNpcBoy01Pb2, NULL },
    { NULL, modelStrNpcBoy01Pb, NULL },
    { NULL, modelStrNpcBoy02Pb2, NULL },
    { NULL, modelStrNpcBoy02Pb, NULL },
    { NULL, modelStrNpcGir01Pb2, NULL },
    { NULL, modelStrNpcGir01Pb, NULL },
    { NULL, modelStrNpcGir02Pb2, NULL },
    { NULL, modelStrNpcGir02Pb, NULL },
    { NULL, modelStrNpcUni01Pb3, NULL },
    { NULL, modelStrNpcUni02Pb, NULL },
    { NULL, modelStrNpcUni03Pb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcUni04Pb, NULL },
    { NULL, modelStrNpcUni05Pb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcUni07Pb, NULL },
    { NULL, modelStrNpcUni01Pb2, NULL },
    { NULL, modelStrNpcUni01Pb, NULL },
    { NULL, modelStrNpcKim01Pb4, NULL },
    { NULL, modelStrNpcKim01Pb3, NULL },
    { NULL, modelStrNpcKim02Pb4, NULL },
    { NULL, modelStrNpcKim02Pb3, NULL },
    { NULL, modelStrNpcHak01Pb2, NULL },
    { NULL, modelStrNpcHak01Pb, NULL },
    { NULL, modelStrNpcKim01Pb2, NULL },
    { NULL, modelStrNpcKim01Pb, NULL },
    { NULL, modelStrNpcKim02Pb2, NULL },
    { NULL, modelStrNpcKim02Pb, NULL },
    { NULL, modelStrNpcKim03Pb2, NULL },
    { NULL, modelStrNpcKim03Pb, NULL },
    { NULL, modelStrNpcYou01Pb4, NULL },
    { NULL, modelStrNpcYou01Pb3, NULL },
    { NULL, modelStrNpcUni04Pb, NULL },
    { NULL, modelStrNpcUni05Pb, NULL },
    { NULL, modelStrNpcYou03Pb4, NULL },
    { NULL, modelStrNpcYou03Pb3, NULL },
    { NULL, modelStrNpcYou05Pb4, NULL },
    { NULL, modelStrNpcYou05Pb3, NULL },
    { NULL, modelStrNpcYou01Pb2, NULL },
    { NULL, modelStrNpcYou01Pb, NULL },
    { NULL, modelStrNpcYou03Pb2, NULL },
    { NULL, modelStrNpcYou03Pb, NULL },
    { NULL, modelStrNpcKim01Pb3, NULL },
    { NULL, modelStrNpcKim02Pb4, NULL },
    { NULL, modelStrNpcYou05Pb2, NULL },
    { NULL, modelStrNpcYou05Pb, NULL },
    { NULL, modelStrNpcJob01Pb4, NULL },
    { NULL, modelStrNpcJob01Pb3, NULL },
    { NULL, modelStrNpcJob02Pb3, NULL },
    { NULL, modelStrNpcJob03Pb3, NULL },
    { NULL, modelStrNpcJob04Pb, NULL },
    { NULL, modelStrNpcJob05Pb, NULL },
    { NULL, modelStrNpcJob06Pb, NULL },
    { NULL, modelStrNpcJob07Pb2, NULL },
    { NULL, modelStrNpcJob07Pb, NULL },
    { NULL, modelStrNpcJob08Pb, NULL },
    { NULL, modelStrNpcKim01Pb, NULL },
    { NULL, modelStrNpcJob09Pb2, NULL },
    { NULL, modelStrNpcKim02Pb2, NULL },
    { NULL, modelStrNpcJob11Pb, NULL },
    { NULL, modelStrNpcJob12Pb3, NULL },
    { NULL, modelStrNpcJob12Pb2, NULL },
    { NULL, modelStrNpcJob12Pb, NULL },
    { NULL, modelStrNpcJob13Pb, NULL },
    { NULL, modelStrNpcKim04Pb2, NULL },
    { NULL, modelStrNpcKim04Pb, NULL },
    { NULL, modelStrNpcJob02Pb2, NULL },
    { NULL, modelStrNpcJob02Pb, NULL },
    { NULL, modelStrNpcJob03Pb2, NULL },
    { NULL, modelStrNpcJob03Pb, NULL },
    { NULL, modelStrNpcYou02Pb, NULL },
    { NULL, modelStrNpcCar01Pb, NULL },
    { NULL, modelStrNpcCar02Pb, NULL },
    { NULL, modelStrNpcCar03Pb, NULL },
    { NULL, modelStrNpcYou03Pb4, NULL },
    { NULL, modelStrNpcTra01Pb2, NULL },
    { NULL, modelStrNpcTra01Pb, NULL },
    { NULL, modelStrNpcDog01Pb2, NULL },
    { NULL, modelStrNpcShi01Pb, NULL },
    { NULL, modelStrNpcYou05Pb3, NULL },
    { NULL, modelStrNpcYou05Pb3, NULL },
    { NULL, modelStrNpcCat01Pb, NULL },
    { NULL, modelStrNpcCat02Pb, NULL },
    { NULL, modelStrNpcCat03Pb, NULL },
    { NULL, modelStrNpcDog01Pb, NULL },
    { NULL, modelStrDevil092Pb, NULL },
    { NULL, modelStrDevil051Pb, NULL },
    { NULL, modelStrDevil036Pb, NULL },
    { NULL, modelStrDevil113Pb, NULL },
    { NULL, modelStrDevil016Pb, NULL },
    { NULL, modelStrDevil00CPb, NULL },
    { NULL, modelStrDevil031Pb, NULL },
    { NULL, modelStrDevil027Pb, NULL },
    { NULL, modelStrDevil122Pb, NULL },
    { NULL, modelStrDevil094Pb, NULL },
    { NULL, modelStrDevil014Pb, NULL },
    { NULL, modelStrDevil02CPb, NULL },
    { NULL, modelStrDevil02BPb, NULL },
    { NULL, modelStrDevil023Pb, NULL },
    { NULL, modelStrDevil095Pb, NULL },
    { NULL, modelStrDevil039Pb, NULL },
    { NULL, modelStrDevil10CPb, NULL },
    { NULL, modelStrDevil10DPb, NULL },
    { NULL, modelStrDevil007Pb, NULL },
    { NULL, modelStrDevil098Pb, NULL },
    { NULL, modelStrDevil10BPb, NULL },
    { NULL, modelStrDevil017Pb, NULL },
    { NULL, modelStrDevil013Pb, NULL },
    { NULL, modelStrDevil012Pb, NULL },
    { NULL, modelStrDevil00BPb, NULL },
    { NULL, modelStrDevil030Pb, NULL },
    { NULL, modelStrNpcJob06Pb, NULL },
    { NULL, modelStrDevil00EPb, NULL },
    { NULL, modelStrDevil010Pb, NULL },
    { NULL, modelStrDevil124Pb, NULL },
    { NULL, modelStrDevil020Pb, NULL },
    { NULL, modelStrDevil036Pb, NULL },
    { NULL, modelStrNpcJimusyo00Pb, NULL },
    { NULL, modelStrNpcJob08Pb, NULL },
    { NULL, modelStrNpcJob09Pb, NULL },
    { NULL, modelStrNpcKonnou00Pb, NULL },
    { NULL, modelStrNpcMilk00Pb, NULL },
    { NULL, modelStrNpcKaya02Pb, NULL },
    { NULL, modelStrEventRasBodyPb, NULL },
    { NULL, modelStrNpcJob10Pb, NULL },
    { NULL, modelStrNpcJob11Pb, NULL },
    { NULL, modelStrNpcJob11Pb, NULL },
    { NULL, modelStrNpcJob01Pb2, NULL },
    { NULL, modelStrNpcJob01Pb2, NULL },
    { NULL, modelStrNpcJob01Pb, NULL },
    { NULL, modelStrNpcJob01Pb, NULL },
    { NULL, modelStrNpcJob02Pb2, NULL },
    { NULL, modelStrNpcJob02Pb2, NULL },
    { NULL, modelStrNpcMilk01Pb, NULL },
    { NULL, modelStrNpcMilk02Pb, NULL },
    { NULL, modelStrNpcMilk03Pb, NULL },
    { NULL, modelStrNpcKityouMilkPb, NULL },
    { NULL, modelStrNpcMilk04Pb, NULL },
    { NULL, modelStrNpcMilk05Pb, NULL },
    { NULL, modelStrNpcRausuMilkEventPb, NULL },
    { NULL, modelStrNpcKityouMilkEventPb, NULL },
    { NULL, modelStrNpcCar01Pb, NULL },
    { NULL, modelStrNpcCar02Pb, NULL },
    { NULL, modelStrNpcCar03Pb, NULL },
    { NULL, modelStrNpcTra01Pb2, NULL },
    { NULL, modelStrNpcTra01Pb, NULL },
    { NULL, modelStrNpcTra02Pb, NULL },
    { NULL, modelStrNpcCat01Pb, NULL },
    { NULL, modelStrNpcDog01Pb, NULL },
    { NULL, modelStrNpcRei01Pb, NULL },
    { NULL, modelStrNpcToolPbPb, NULL },
    { NULL, modelStrNpcObj01Pb, NULL },
    { NULL, modelStrNpcObj02Pb, NULL },
    { NULL, modelStrNpcObj03Pb, NULL },
    { NULL, modelStrNpcObj04Pb, NULL },
    { NULL, modelStrNpcObj05Pb, NULL },
    { NULL, modelStrNpcObj06Pb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcShi02Pb2, NULL },
    { NULL, modelStrNpcShi02Pb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrDevil110Pb, NULL },
    { NULL, modelStrDevil113Pb, NULL },
    { NULL, modelStrDevil114Pb, NULL },
    { NULL, modelStrDevil115Pb, NULL },
    { NULL, modelStrDevil025Pb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
    { NULL, modelStrNpcToolPb, NULL },
};

dds3ModelFileTable_t modelFileTableEntry6[81] = {
    { NULL, modelStrDevilHi000HiPb, NULL },
    { NULL, modelStrDevilHi001HiPb, NULL },
    { NULL, modelStrDevilHi002HiPb, NULL },
    { NULL, modelStrDevilHi003HiPb, NULL },
    { NULL, modelStrDevilHi004HiPb, NULL },
    { NULL, modelStrDevilHi005HiPb, NULL },
    { NULL, modelStrDevilHi006HiPb, NULL },
    { NULL, modelStrDevilHi007HiPb, NULL },
    { NULL, modelStrDevilHi008HiPb, NULL },
    { NULL, modelStrDevilHi009HiPb, NULL },
    { NULL, modelStrDevilHi00AHiPb, NULL },
    { NULL, modelStrDevilHi00BHiPb, NULL },
    { NULL, modelStrDevilHi00CHiPb, NULL },
    { NULL, modelStrDevilHi00DHiPb, NULL },
    { NULL, modelStrDevilHi00EHiPb, NULL },
    { NULL, modelStrDevilHi00FHiPb, NULL },
    { NULL, modelStrDevilHi010HiPb, NULL },
    { NULL, modelStrDevilHi011HiPb, NULL },
    { NULL, modelStrDevilHi012HiPb, NULL },
    { NULL, modelStrDevilHi013HiPb, NULL },
    { NULL, modelStrDevilHi014HiPb, NULL },
    { NULL, modelStrDevilHi015HiPb, NULL },
    { NULL, modelStrDevilHi016HiPb, NULL },
    { NULL, modelStrDevilHi017HiPb, NULL },
    { NULL, modelStrDevilHi018HiPb, NULL },
    { NULL, modelStrDevilHi019HiPb, NULL },
    { NULL, modelStrDevilHi01AHiPb, NULL },
    { NULL, modelStrDevilHi01BHiPb, NULL },
    { NULL, modelStrDevilHi01CHiPb, NULL },
    { NULL, modelStrDevilHi01DHiPb, NULL },
    { NULL, modelStrDevilHi01EHiPb, NULL },
    { NULL, modelStrDevilHi01FHiPb, NULL },
    { NULL, modelStrDevilHi020HiPb, NULL },
    { NULL, modelStrDevilHi021HiPb, NULL },
    { NULL, modelStrDevilHi022HiPb, NULL },
    { NULL, modelStrDevilHi023HiPb, NULL },
    { NULL, modelStrDevilHi024HiPb, NULL },
    { NULL, modelStrDevilHi025HiPb, NULL },
    { NULL, modelStrDevilHi026HiPb, NULL },
    { NULL, modelStrDevilHi027HiPb, NULL },
    { NULL, modelStrDevilHi028HiPb, NULL },
    { NULL, modelStrDevilHi029HiPb, NULL },
    { NULL, modelStrDevilHi02AHiPb, NULL },
    { NULL, modelStrDevilHi02BHiPb, NULL },
    { NULL, modelStrDevilHi02CHiPb, NULL },
    { NULL, modelStrDevilHi02DHiPb, NULL },
    { NULL, modelStrDevilHi02EHiPb, NULL },
    { NULL, modelStrDevilHi02FHiPb, NULL },
    { NULL, modelStrDevilHi030HiPb, NULL },
    { NULL, modelStrDevilHi031HiPb, NULL },
    { NULL, modelStrDevilHi032HiPb, NULL },
    { NULL, modelStrDevilHi033HiPb, NULL },
    { NULL, modelStrDevilHi034HiPb, NULL },
    { NULL, modelStrDevilHi035HiPb, NULL },
    { NULL, modelStrDevilHi036HiPb, NULL },
    { NULL, modelStrDevilHi037HiPb, NULL },
    { NULL, modelStrDevilHi038HiPb, NULL },
    { NULL, modelStrDevilHi039HiPb, NULL },
    { NULL, modelStrDevilHi03AHiPb, NULL },
    { NULL, modelStrDevilHi03BHiPb, NULL },
    { NULL, modelStrDevilHi03CHiPb, NULL },
    { NULL, modelStrDevilHi03DHiPb, NULL },
    { NULL, modelStrDevilHi03EHiPb, NULL },
    { NULL, modelStrDevilHi03FHiPb, NULL },
    { NULL, modelStrDevilHi040HiPb, NULL },
    { NULL, modelStrDevilHi041HiPb, NULL },
    { NULL, modelStrDevilHi042HiPb, NULL },
    { NULL, modelStrDevilHi043HiPb, NULL },
    { NULL, modelStrDevilHi044HiPb, NULL },
    { NULL, modelStrDevilHi045HiPb, NULL },
    { NULL, modelStrDevilHi046HiPb, NULL },
    { NULL, modelStrDevilHi047HiPb, NULL },
    { NULL, modelStrDevilHi021HiPb, NULL },
    { NULL, modelStrDevilHi021HiPb, NULL },
    { NULL, modelStrDevilHi021HiPb, NULL },
    { NULL, modelStrDevilHi021HiPb, NULL },
    { NULL, modelStrDevilHiTiityaPb, NULL },
    { NULL, modelStrDevilHiTenbouPb, NULL },
    { NULL, modelStrDevilHiHaiPb, NULL },
    { NULL, modelStrDevilHiTakuPb, NULL },
    { NULL, modelStrDevilHiNarumiPb, NULL },
};

dds3ModelFileTable_t modelFileTableEntry7[13] = {
    { pbFileMiscColorbarTB, pbFileMiscColorbarMB, pbFileMiscColorbarAB },
    { NULL, pbFileMiscGyukiPB, NULL },
    { NULL, pbFileMiscInkyuPB, NULL },
    { NULL, pbFileMiscKoronPB, NULL },
    { NULL, pbFileMiscPapiruPB, NULL },
    { NULL, pbFileMiscElfPB, NULL },
    { NULL, pbFileMiscDoriPB, NULL },
    { NULL, pbFileMiscKariahaPB, NULL },
    { NULL, pbFileMiscBukabuPB, NULL },
    { NULL, modelStrEventTestSurfPb, NULL },
    { NULL, modelStrEventTestHeatPb, NULL },
    { NULL, modelStrEventTestAljiraPb, NULL },
    { NULL, modelStrEventTestGale2Pb, NULL },
};

mdlFileSlot_t modelFileTable[8] = {
    { modelFileTableEntry, 34 },
    { modelFileTableEntry2, 352 },
    { modelFileTableEntry4, 80 },
    { modelFileTableEntry5, 224 },
    { modelFileTableEntry7, 13 },
    { NULL, 0 },
    { modelFileTableEntry3, 80 },
    { modelFileTableEntry6, 81 },
};

dds3ModelFileIndex_t modelFileKindTableEntry[34] = {
    { 0, 0, 0x1000, 1000 },
    { 0, 1, 0x1000, 1000 },
    { 0, 2, 0x1000, 1000 },
    { 0, 3, 0x1000, 1000 },
    { 0, 4, 0x1000, 1000 },
    { 0, 5, 0x1000, 1000 },
    { 0, 6, 0x1000, 1000 },
    { 0, 7, 0x1000, 1000 },
    { 0, 8, 0x1000, 1000 },
    { 0, 9, 0x1000, 1000 },
    { 0, 10, 0x1000, 1000 },
    { 0, 11, 0x1000, 1000 },
    { 0, 12, 0x1000, 1000 },
    { 0, 13, 0x1000, 1000 },
    { 0, 14, 0x1000, 1000 },
    { 0, 15, 0x1000, 1000 },
    { 0, 16, 0x1000, 1000 },
    { 0, 17, 0x1000, 1000 },
    { 0, 18, 0x1000, 1000 },
    { 0, 19, 0x1000, 1000 },
    { 0, 20, 0x1000, 1000 },
    { 0, 21, 0x1000, 1000 },
    { 0, 22, 0x1000, 1000 },
    { 0, 23, 0x1000, 1000 },
    { 0, 24, 0x1000, 1000 },
    { 0, 25, 0x1000, 1000 },
    { 0, 26, 0x1000, 1000 },
    { 0, 27, 0x1000, 1000 },
    { 0, 28, 0x1000, 1000 },
    { 0, 29, 0x1000, 1000 },
    { 0, 30, 0x1000, 1000 },
    { 0, 31, 0x1000, 1000 },
    { 0, 32, 0x1000, 1000 },
    { 0, 33, 0x1000, 1000 },
};

dds3ModelFileIndex_t mdlGetNumOfMinorDataEntry[352] = {
    { 1, 0, 0x1000, 1000 },
    { 1, 1, 0x1000, 1000 },
    { 1, 2, 0x1000, 1000 },
    { 1, 3, 0x1000, 1000 },
    { 1, 4, 0x1000, 1000 },
    { 1, 5, 0x1000, 1000 },
    { 1, 6, 0x1000, 1000 },
    { 1, 7, 0x1000, 1000 },
    { 1, 8, 0x1000, 1000 },
    { 1, 9, 0x1000, 1000 },
    { 1, 10, 0x1000, 1000 },
    { 1, 11, 0x1000, 1000 },
    { 1, 12, 0x1000, 1000 },
    { 1, 13, 0x1000, 1000 },
    { 1, 14, 0x1000, 1000 },
    { 1, 15, 0x1000, 1000 },
    { 1, 16, 0x1000, 1000 },
    { 1, 17, 0x1000, 1000 },
    { 1, 18, 0x1000, 1000 },
    { 1, 19, 0x1000, 1000 },
    { 1, 20, 0x1000, 1000 },
    { 1, 21, 0x1000, 1000 },
    { 1, 22, 0x1000, 1000 },
    { 1, 23, 0x1000, 1000 },
    { 1, 24, 0x1000, 1000 },
    { 1, 25, 0x1000, 1000 },
    { 1, 26, 0x1000, 1000 },
    { 1, 27, 0x1000, 1000 },
    { 1, 28, 0x1000, 1000 },
    { 1, 29, 0x1000, 1000 },
    { 1, 30, 0x1000, 1000 },
    { 1, 31, 0x1000, 1000 },
    { 1, 32, 0x1000, 1000 },
    { 1, 33, 0x1000, 1000 },
    { 1, 34, 0x1000, 1000 },
    { 1, 35, 0x1000, 1000 },
    { 1, 36, 0x1000, 1000 },
    { 1, 37, 0x1000, 1000 },
    { 1, 38, 0x1000, 1000 },
    { 1, 39, 0x1000, 1000 },
    { 1, 40, 0x1000, 1000 },
    { 1, 41, 0x1000, 1000 },
    { 1, 42, 0x1000, 1000 },
    { 1, 43, 0x1000, 1000 },
    { 1, 44, 0x1000, 1000 },
    { 1, 45, 0x1000, 1000 },
    { 1, 46, 0x1000, 1000 },
    { 1, 47, 0x1000, 1000 },
    { 1, 48, 0x1000, 1000 },
    { 1, 49, 0x1000, 1000 },
    { 1, 50, 0x1000, 1000 },
    { 1, 51, 0x1000, 1000 },
    { 1, 52, 0x1000, 1000 },
    { 1, 53, 0x1000, 1000 },
    { 1, 54, 0x1000, 1000 },
    { 1, 55, 0x1000, 1000 },
    { 1, 56, 0x1000, 1000 },
    { 1, 57, 0x1000, 1000 },
    { 1, 58, 0x1000, 1000 },
    { 1, 59, 0x1000, 1000 },
    { 1, 60, 0x1000, 1000 },
    { 1, 61, 0x1000, 1000 },
    { 1, 62, 0x1000, 1000 },
    { 1, 63, 0x1000, 1000 },
    { 1, 64, 0x1000, 1000 },
    { 1, 65, 0x1000, 1000 },
    { 1, 66, 0x1000, 1000 },
    { 1, 67, 0x1000, 1000 },
    { 1, 68, 0x1000, 1000 },
    { 1, 69, 0x1000, 1000 },
    { 1, 70, 0x1000, 1000 },
    { 1, 71, 0x1000, 1000 },
    { 1, 72, 0x1000, 1000 },
    { 1, 73, 0x1000, 1000 },
    { 1, 74, 0x1000, 1000 },
    { 1, 75, 0x1000, 1000 },
    { 1, 76, 0x1000, 1000 },
    { 1, 77, 0x1000, 1000 },
    { 1, 78, 0x1000, 1000 },
    { 1, 79, 0x1000, 1000 },
    { 1, 80, 0x1000, 1000 },
    { 1, 81, 0x1000, 1000 },
    { 1, 82, 0x1000, 1000 },
    { 1, 83, 0x1000, 1000 },
    { 1, 84, 0x1000, 1000 },
    { 1, 85, 0x1000, 1000 },
    { 1, 86, 0x1000, 1000 },
    { 1, 87, 0x1000, 1000 },
    { 1, 88, 0x1000, 1000 },
    { 1, 89, 0x1000, 1000 },
    { 1, 90, 0x1000, 1000 },
    { 1, 91, 0x1000, 1000 },
    { 1, 92, 0x1000, 1000 },
    { 1, 93, 0x1000, 1000 },
    { 1, 94, 0x1000, 1000 },
    { 1, 95, 0x1000, 1000 },
    { 1, 96, 0x1000, 1000 },
    { 1, 97, 0x1000, 1000 },
    { 1, 98, 0x1000, 1000 },
    { 1, 99, 0x1000, 1000 },
    { 1, 100, 0x1000, 1000 },
    { 1, 101, 0x1000, 1000 },
    { 1, 102, 0x1000, 1000 },
    { 1, 103, 0x1000, 1000 },
    { 1, 104, 0x1000, 1000 },
    { 1, 105, 0x1000, 1000 },
    { 1, 106, 0x1000, 1000 },
    { 1, 107, 0x1000, 1000 },
    { 1, 108, 0x1000, 1000 },
    { 1, 109, 0x1000, 1000 },
    { 1, 110, 0x1000, 1000 },
    { 1, 111, 0x1000, 1000 },
    { 1, 112, 0x1000, 1000 },
    { 1, 113, 0x1000, 1000 },
    { 1, 114, 0x1000, 1000 },
    { 1, 115, 0x1000, 1000 },
    { 1, 116, 0x1000, 1000 },
    { 1, 117, 0x1000, 1000 },
    { 1, 118, 0x1000, 1000 },
    { 1, 119, 0x1000, 1000 },
    { 1, 120, 0x1000, 1000 },
    { 1, 121, 0x1000, 1000 },
    { 1, 122, 0x1000, 1000 },
    { 1, 123, 0x1000, 1000 },
    { 1, 124, 0x1000, 1000 },
    { 1, 125, 0x1000, 1000 },
    { 1, 126, 0x1000, 1000 },
    { 1, 127, 0x1000, 1000 },
    { 1, 128, 0x1000, 1000 },
    { 1, 129, 0x1000, 1000 },
    { 1, 130, 0x1000, 1000 },
    { 1, 131, 0x1000, 1000 },
    { 1, 132, 0x1000, 1000 },
    { 1, 133, 0x1000, 1000 },
    { 1, 134, 0x1000, 1000 },
    { 1, 135, 0x1000, 1000 },
    { 1, 136, 0x1000, 1000 },
    { 1, 137, 0x1000, 1000 },
    { 1, 138, 0x1000, 1000 },
    { 1, 139, 0x1000, 1000 },
    { 1, 140, 0x1000, 1000 },
    { 1, 141, 0x1000, 1000 },
    { 1, 142, 0x1000, 1000 },
    { 1, 143, 0x1000, 1000 },
    { 1, 144, 0x1000, 1000 },
    { 1, 145, 0x1000, 1000 },
    { 1, 146, 0x1000, 1000 },
    { 1, 147, 0x1000, 1000 },
    { 1, 148, 0x1000, 1000 },
    { 1, 149, 0x1000, 1000 },
    { 1, 150, 0x1000, 1000 },
    { 1, 151, 0x1000, 1000 },
    { 1, 152, 0x1000, 1000 },
    { 1, 153, 0x1000, 1000 },
    { 1, 154, 0x1000, 1000 },
    { 1, 155, 0x1000, 1000 },
    { 1, 156, 0x1000, 1000 },
    { 1, 157, 0x1000, 1000 },
    { 1, 158, 0x1000, 1000 },
    { 1, 159, 0x1000, 1000 },
    { 1, 160, 0x1000, 1000 },
    { 1, 161, 0x1000, 1000 },
    { 1, 162, 0x1000, 1000 },
    { 1, 163, 0x1000, 1000 },
    { 1, 164, 0x1000, 2500 },
    { 1, 165, 0x1000, 2500 },
    { 1, 166, 0x1000, 2500 },
    { 1, 167, 0x1000, 2500 },
    { 1, 168, 0x1000, 2500 },
    { 1, 169, 0x1000, 2500 },
    { 1, 170, 0x1000, 2500 },
    { 1, 171, 0x1000, 2500 },
    { 1, 172, 0x1000, 2500 },
    { 1, 173, 0x1000, 2500 },
    { 1, 174, 0x1000, 2500 },
    { 1, 175, 0x1000, 2500 },
    { 1, 176, 0x1000, 2500 },
    { 1, 177, 0x1000, 2500 },
    { 1, 178, 0x1000, 2500 },
    { 1, 179, 0x1000, 2500 },
    { 1, 180, 0x1000, 2500 },
    { 1, 181, 0x1000, 2500 },
    { 1, 182, 0x1000, 2500 },
    { 1, 183, 0x1000, 2500 },
    { 1, 184, 0x1000, 2500 },
    { 1, 185, 0x1000, 2500 },
    { 1, 186, 0x1000, 2500 },
    { 1, 187, 0x1000, 2500 },
    { 1, 188, 0x1000, 2500 },
    { 1, 189, 0x1000, 2500 },
    { 1, 190, 0x1000, 2500 },
    { 1, 191, 0x1000, 2500 },
    { 1, 192, 0x1000, 2500 },
    { 1, 193, 0x1000, 2500 },
    { 1, 194, 0x1000, 2500 },
    { 1, 195, 0x1000, 2500 },
    { 1, 196, 0x1000, 2500 },
    { 1, 197, 0x1000, 2500 },
    { 1, 198, 0x1000, 2500 },
    { 1, 199, 0x1000, 2500 },
    { 1, 200, 0x1000, 2500 },
    { 1, 201, 0x1000, 2500 },
    { 1, 202, 0x1000, 2500 },
    { 1, 203, 0x1000, 2500 },
    { 1, 204, 0x1000, 2500 },
    { 1, 205, 0x1000, 2500 },
    { 1, 206, 0x1000, 2500 },
    { 1, 207, 0x1000, 2500 },
    { 1, 208, 0x1000, 2500 },
    { 1, 209, 0x1000, 2500 },
    { 1, 210, 0x1000, 2500 },
    { 1, 211, 0x1000, 2500 },
    { 1, 212, 0x1000, 2500 },
    { 1, 213, 0x1000, 2500 },
    { 1, 214, 0x1000, 2500 },
    { 1, 215, 0x1000, 2500 },
    { 1, 216, 0x1000, 2500 },
    { 1, 217, 0x1000, 2500 },
    { 1, 218, 0x1000, 2500 },
    { 1, 219, 0x1000, 2500 },
    { 1, 220, 0x1000, 2500 },
    { 1, 221, 0x1000, 2500 },
    { 1, 222, 0x1000, 2500 },
    { 1, 223, 0x1000, 2500 },
    { 1, 224, 0x1000, 2500 },
    { 1, 225, 0x1000, 2500 },
    { 1, 226, 0x1000, 2500 },
    { 1, 227, 0x1000, 2500 },
    { 1, 228, 0x1000, 2500 },
    { 1, 229, 0x1000, 2500 },
    { 1, 230, 0x1000, 2500 },
    { 1, 231, 0x1000, 2500 },
    { 1, 232, 0x1000, 2500 },
    { 1, 233, 0x1000, 2500 },
    { 1, 234, 0x1000, 2500 },
    { 1, 235, 0x1000, 2500 },
    { 1, 236, 0x1000, 2500 },
    { 1, 237, 0x1000, 2500 },
    { 1, 238, 0x1000, 2500 },
    { 1, 239, 0x1000, 2500 },
    { 1, 240, 0x1000, 2500 },
    { 1, 241, 0x1000, 2500 },
    { 1, 242, 0x1000, 2500 },
    { 1, 243, 0x1000, 2500 },
    { 1, 244, 0x1000, 2500 },
    { 1, 245, 0x1000, 2500 },
    { 1, 246, 0x1000, 2500 },
    { 1, 247, 0x1000, 2500 },
    { 1, 248, 0x1000, 2500 },
    { 1, 249, 0x1000, 2500 },
    { 1, 250, 0x1000, 2500 },
    { 1, 251, 0x1000, 2500 },
    { 1, 252, 0x1000, 2500 },
    { 1, 253, 0x1000, 2500 },
    { 1, 254, 0x1000, 2500 },
    { 1, 255, 0x1000, 2500 },
    { 1, 256, 0x1000, 2500 },
    { 1, 257, 0x1000, 2500 },
    { 1, 258, 0x1000, 2500 },
    { 1, 259, 0x1000, 2500 },
    { 1, 260, 0x1000, 2500 },
    { 1, 261, 0x1000, 2500 },
    { 1, 262, 0x1000, 2500 },
    { 1, 263, 0x1000, 2500 },
    { 1, 264, 0x1000, 2500 },
    { 1, 265, 0x1000, 2500 },
    { 1, 266, 0x1000, 2500 },
    { 1, 267, 0x1000, 2500 },
    { 1, 268, 0x1000, 2500 },
    { 1, 269, 0x1000, 2500 },
    { 1, 270, 0x1000, 2500 },
    { 1, 271, 0x1000, 2500 },
    { 1, 272, 0x1000, 2500 },
    { 1, 273, 0x1000, 2500 },
    { 1, 274, 0x1000, 2500 },
    { 1, 275, 0x1000, 2500 },
    { 1, 276, 0x1000, 2500 },
    { 1, 277, 0x1000, 2500 },
    { 1, 278, 0x1000, 2500 },
    { 1, 279, 0x1000, 2500 },
    { 1, 280, 0x1000, 2500 },
    { 1, 281, 0x1000, 2500 },
    { 1, 282, 0x1000, 2500 },
    { 1, 283, 0x1000, 2500 },
    { 1, 284, 0x1000, 2500 },
    { 1, 285, 0x1000, 2500 },
    { 1, 286, 0x1000, 2500 },
    { 1, 287, 0x1000, 2500 },
    { 1, 288, 0x1000, 2500 },
    { 1, 289, 0x1000, 2500 },
    { 1, 290, 0x1000, 2500 },
    { 1, 291, 0x1000, 2500 },
    { 1, 292, 0x1000, 2500 },
    { 1, 293, 0x1000, 2500 },
    { 1, 294, 0x1000, 2500 },
    { 1, 295, 0x1000, 2500 },
    { 1, 296, 0x1000, 2500 },
    { 1, 297, 0x1000, 2500 },
    { 1, 298, 0x1000, 2500 },
    { 1, 299, 0x1000, 2500 },
    { 1, 300, 0x1000, 2500 },
    { 1, 301, 0x1000, 2500 },
    { 1, 302, 0x1000, 2500 },
    { 1, 303, 0x1000, 2500 },
    { 1, 304, 0x1000, 2500 },
    { 1, 305, 0x1000, 2500 },
    { 1, 306, 0x1000, 2500 },
    { 1, 307, 0x1000, 2500 },
    { 1, 308, 0x1000, 2500 },
    { 1, 309, 0x1000, 2500 },
    { 1, 310, 0x1000, 2500 },
    { 1, 311, 0x1000, 2500 },
    { 1, 312, 0x1000, 2500 },
    { 1, 313, 0x1000, 2500 },
    { 1, 314, 0x1000, 2500 },
    { 1, 315, 0x1000, 2500 },
    { 1, 316, 0x1000, 2500 },
    { 1, 317, 0x1000, 2500 },
    { 1, 318, 0x1000, 2500 },
    { 1, 319, 0x1000, 2500 },
    { 1, 320, 0x1000, 2500 },
    { 1, 321, 0x1000, 2500 },
    { 1, 322, 0x1000, 2500 },
    { 1, 323, 0x1000, 2500 },
    { 1, 324, 0x1000, 2500 },
    { 1, 325, 0x1000, 2500 },
    { 1, 326, 0x1000, 2500 },
    { 1, 327, 0x1000, 2500 },
    { 1, 328, 0x1000, 2500 },
    { 1, 329, 0x1000, 2500 },
    { 1, 330, 0x1000, 2500 },
    { 1, 331, 0x1000, 2500 },
    { 1, 332, 0x1000, 2500 },
    { 1, 333, 0x1000, 2500 },
    { 1, 334, 0x1000, 2500 },
    { 1, 335, 0x1000, 2500 },
    { 1, 336, 0x1000, 2500 },
    { 1, 337, 0x1000, 2500 },
    { 1, 338, 0x1000, 2500 },
    { 1, 339, 0x1000, 2500 },
    { 1, 340, 0x1000, 2500 },
    { 1, 341, 0x1000, 2500 },
    { 1, 342, 0x1000, 2500 },
    { 1, 343, 0x1000, 2500 },
    { 1, 344, 0x1000, 2500 },
    { 1, 345, 0x1000, 2500 },
    { 1, 346, 0x1000, 2500 },
    { 1, 347, 0x1000, 2500 },
    { 1, 348, 0x1000, 2500 },
    { 1, 349, 0x1000, 2500 },
    { 1, 350, 0x1000, 2500 },
    { 1, 351, 0x1000, 2500 },
};

dds3ModelFileIndex_t mdlGetNumOfMinorDataEntry2[80] = {
    { 6, 0, 0x1000, 1000 },
    { 6, 1, 0x1000, 1000 },
    { 6, 2, 0x1000, 1000 },
    { 6, 3, 0x1000, 1000 },
    { 6, 4, 0x1000, 1000 },
    { 6, 5, 0x1000, 1000 },
    { 6, 6, 0x1000, 1000 },
    { 6, 7, 0x1000, 1000 },
    { 6, 8, 0x1000, 1000 },
    { 6, 9, 0x1000, 1000 },
    { 6, 10, 0x1000, 1000 },
    { 6, 11, 0x1000, 1000 },
    { 6, 12, 0x1000, 1000 },
    { 6, 13, 0x1000, 1000 },
    { 6, 14, 0x1000, 1000 },
    { 6, 15, 0x1000, 1000 },
    { 6, 16, 0x1000, 1000 },
    { 6, 17, 0x1000, 1000 },
    { 6, 18, 0x1000, 1000 },
    { 6, 19, 0x1000, 1000 },
    { 6, 20, 0x1000, 1000 },
    { 6, 21, 0x1000, 1000 },
    { 6, 22, 0x1000, 1000 },
    { 6, 23, 0x1000, 1000 },
    { 6, 24, 0x1000, 1000 },
    { 6, 25, 0x1000, 1000 },
    { 6, 26, 0x1000, 1000 },
    { 6, 27, 0x1000, 1000 },
    { 6, 28, 0x1000, 1000 },
    { 6, 29, 0x1000, 1000 },
    { 6, 30, 0x1000, 1000 },
    { 6, 31, 0x1000, 1000 },
    { 6, 32, 0x1000, 1000 },
    { 6, 33, 0x1000, 1000 },
    { 6, 34, 0x1000, 1000 },
    { 6, 35, 0x1000, 1000 },
    { 6, 36, 0x1000, 1000 },
    { 6, 37, 0x1000, 1000 },
    { 6, 38, 0x1000, 1000 },
    { 6, 39, 0x1000, 1000 },
    { 6, 40, 0x1000, 1000 },
    { 6, 41, 0x1000, 1000 },
    { 6, 42, 0x1000, 1000 },
    { 6, 43, 0x1000, 1000 },
    { 6, 44, 0x1000, 1000 },
    { 6, 45, 0x1000, 1000 },
    { 6, 46, 0x1000, 1000 },
    { 6, 47, 0x1000, 1000 },
    { 6, 48, 0x1000, 1000 },
    { 6, 49, 0x1000, 1000 },
    { 6, 50, 0x1000, 1000 },
    { 6, 51, 0x1000, 1000 },
    { 6, 52, 0x1000, 1000 },
    { 6, 53, 0x1000, 1000 },
    { 6, 54, 0x1000, 1000 },
    { 6, 55, 0x1000, 1000 },
    { 6, 56, 0x1000, 1000 },
    { 6, 57, 0x1000, 1000 },
    { 6, 58, 0x1000, 1000 },
    { 6, 59, 0x1000, 1000 },
    { 6, 60, 0x1000, 1000 },
    { 6, 61, 0x1000, 1000 },
    { 6, 62, 0x1000, 1000 },
    { 6, 63, 0x1000, 1000 },
    { 6, 64, 0x1000, 1000 },
    { 6, 65, 0x1000, 1000 },
    { 6, 66, 0x1000, 1000 },
    { 6, 67, 0x1000, 1000 },
    { 6, 68, 0x1000, 1000 },
    { 6, 69, 0x1000, 1000 },
    { 6, 70, 0x1000, 1000 },
    { 6, 71, 0x1000, 1000 },
    { 6, 72, 0x1000, 1000 },
    { 6, 73, 0x1000, 1000 },
    { 6, 74, 0x1000, 1000 },
    { 6, 75, 0x1000, 1000 },
    { 6, 76, 0x1000, 1000 },
    { 6, 77, 0x1000, 1000 },
    { 6, 78, 0x1000, 1000 },
    { 6, 79, 0x1000, 1000 },
};

dds3ModelFileIndex_t mdlGetNumOfMinorDataEntry3[80] = {
    { 2, 0, 0x1000, 1000 },
    { 2, 1, 0x1000, 1000 },
    { 2, 2, 0x1000, 1000 },
    { 2, 3, 0x1000, 1000 },
    { 2, 4, 0x1000, 1000 },
    { 2, 5, 0x1000, 1000 },
    { 2, 6, 0x1000, 1000 },
    { 2, 7, 0x1000, 1000 },
    { 2, 8, 0x1000, 1000 },
    { 2, 9, 0x1000, 1000 },
    { 2, 10, 0x1000, 1000 },
    { 2, 11, 0x1000, 1000 },
    { 2, 12, 0x1000, 1000 },
    { 2, 13, 0x1000, 1000 },
    { 2, 14, 0x1000, 1000 },
    { 2, 15, 0x1000, 1000 },
    { 2, 16, 0x1000, 1000 },
    { 2, 17, 0x1000, 1000 },
    { 2, 18, 0x1000, 1000 },
    { 2, 19, 0x1000, 1000 },
    { 2, 20, 0x1000, 1000 },
    { 2, 21, 0x1000, 1000 },
    { 2, 22, 0x1000, 1000 },
    { 2, 23, 0x1000, 1000 },
    { 2, 24, 0x1000, 1000 },
    { 2, 25, 0x1000, 1000 },
    { 2, 26, 0x1000, 1000 },
    { 2, 27, 0x1000, 1000 },
    { 2, 28, 0x1000, 1000 },
    { 2, 29, 0x1000, 1000 },
    { 2, 30, 0x1000, 1000 },
    { 2, 31, 0x1000, 1000 },
    { 2, 32, 0x1000, 1000 },
    { 2, 33, 0x1000, 1000 },
    { 2, 34, 0x1000, 1000 },
    { 2, 35, 0x1000, 1000 },
    { 2, 36, 0x1000, 1000 },
    { 2, 37, 0x1000, 1000 },
    { 2, 38, 0x1000, 1000 },
    { 2, 39, 0x1000, 1000 },
    { 2, 40, 0x1000, 1000 },
    { 2, 41, 0x1000, 1000 },
    { 2, 42, 0x1000, 1000 },
    { 2, 43, 0x1000, 1000 },
    { 2, 44, 0x1000, 1000 },
    { 2, 45, 0x1000, 1000 },
    { 2, 46, 0x1000, 1000 },
    { 2, 47, 0x1000, 1000 },
    { 2, 48, 0x1000, 1000 },
    { 2, 49, 0x1000, 1000 },
    { 2, 50, 0x1000, 1000 },
    { 2, 51, 0x1000, 1000 },
    { 2, 52, 0x1000, 1000 },
    { 2, 53, 0x1000, 1000 },
    { 2, 54, 0x1000, 1000 },
    { 2, 55, 0x1000, 1000 },
    { 2, 56, 0x1000, 1000 },
    { 2, 57, 0x1000, 1000 },
    { 2, 58, 0x1000, 1000 },
    { 2, 59, 0x1000, 1000 },
    { 2, 60, 0x1000, 1000 },
    { 2, 61, 0x1000, 1000 },
    { 2, 62, 0x1000, 1000 },
    { 2, 63, 0x1000, 1000 },
    { 2, 64, 0x1000, 1000 },
    { 2, 65, 0x1000, 1000 },
    { 2, 66, 0x1000, 1000 },
    { 2, 67, 0x1000, 1000 },
    { 2, 68, 0x1000, 1000 },
    { 2, 69, 0x1000, 1000 },
    { 2, 70, 0x1000, 1000 },
    { 2, 71, 0x1000, 1000 },
    { 2, 72, 0x1000, 1000 },
    { 2, 73, 0x1000, 1000 },
    { 2, 74, 0x1000, 1000 },
    { 2, 75, 0x1000, 1000 },
    { 2, 76, 0x1000, 1000 },
    { 2, 77, 0x1000, 1000 },
    { 2, 78, 0x1000, 1000 },
    { 2, 79, 0x1000, 1000 },
};

dds3ModelFileIndex_t mdlGetNumOfMinorDataEntry4[224] = {
    { 3, 0, 0x1000, 1000 },
    { 3, 1, 0x1000, 1000 },
    { 3, 2, 0x1000, 1000 },
    { 3, 3, 0x1000, 1000 },
    { 3, 4, 0x1000, 1000 },
    { 3, 5, 0x1000, 1000 },
    { 3, 6, 0x1000, 1000 },
    { 3, 7, 0x1000, 1000 },
    { 3, 8, 0x1000, 1000 },
    { 3, 9, 0x1000, 1000 },
    { 3, 10, 0x1000, 1000 },
    { 3, 11, 0x1000, 1000 },
    { 3, 12, 0x1000, 1000 },
    { 3, 13, 0x1000, 1000 },
    { 3, 14, 0x1000, 1000 },
    { 3, 15, 0x1000, 1000 },
    { 3, 16, 0x1000, 1000 },
    { 3, 17, 0x1000, 1000 },
    { 3, 18, 0x1000, 1000 },
    { 3, 19, 0x1000, 1000 },
    { 3, 20, 0x1000, 1000 },
    { 3, 21, 0x1000, 1000 },
    { 3, 22, 0x1000, 1000 },
    { 3, 23, 0x1000, 1000 },
    { 3, 24, 0x1000, 1000 },
    { 3, 25, 0x1000, 1000 },
    { 3, 26, 0x1000, 1000 },
    { 3, 27, 0x1000, 1000 },
    { 3, 28, 0x1000, 1000 },
    { 3, 29, 0x1000, 1000 },
    { 3, 30, 0x1000, 1000 },
    { 3, 31, 0x1000, 1000 },
    { 3, 32, 0x1000, 1000 },
    { 3, 33, 0x1000, 1000 },
    { 3, 34, 0x1000, 1000 },
    { 3, 35, 0x1000, 1000 },
    { 3, 36, 0x1000, 1000 },
    { 3, 37, 0x1000, 1000 },
    { 3, 38, 0x1000, 1000 },
    { 3, 39, 0x1000, 1000 },
    { 3, 40, 0x1000, 1000 },
    { 3, 41, 0x1000, 1000 },
    { 3, 42, 0x1000, 1000 },
    { 3, 43, 0x1000, 1000 },
    { 3, 44, 0x1000, 1000 },
    { 3, 45, 0x1000, 1000 },
    { 3, 46, 0x1000, 1000 },
    { 3, 47, 0x1000, 1000 },
    { 3, 48, 0x1000, 1000 },
    { 3, 49, 0x1000, 1000 },
    { 3, 50, 0x1000, 1000 },
    { 3, 51, 0x1000, 1000 },
    { 3, 52, 0x1000, 1000 },
    { 3, 53, 0x1000, 1000 },
    { 3, 54, 0x1000, 1000 },
    { 3, 55, 0x1000, 1000 },
    { 3, 56, 0x1000, 1000 },
    { 3, 57, 0x1000, 1000 },
    { 3, 58, 0x1000, 1000 },
    { 3, 59, 0x1000, 1000 },
    { 3, 60, 0x1000, 1000 },
    { 3, 61, 0x1000, 1000 },
    { 3, 62, 0x1000, 1000 },
    { 3, 63, 0x1000, 1000 },
    { 3, 64, 0x1000, 1000 },
    { 3, 65, 0x1000, 1000 },
    { 3, 66, 0x1000, 1000 },
    { 3, 67, 0x1000, 1000 },
    { 3, 68, 0x1000, 1000 },
    { 3, 69, 0x1000, 1000 },
    { 3, 70, 0x1000, 1000 },
    { 3, 71, 0x1000, 1000 },
    { 3, 72, 0x1000, 1000 },
    { 3, 73, 0x1000, 1000 },
    { 3, 74, 0x1000, 1000 },
    { 3, 75, 0x1000, 1000 },
    { 3, 76, 0x1000, 1000 },
    { 3, 77, 0x1000, 1000 },
    { 3, 78, 0x1000, 1000 },
    { 3, 79, 0x1000, 1000 },
    { 3, 80, 0x1000, 1000 },
    { 3, 81, 0x1000, 1000 },
    { 3, 82, 0x1000, 1000 },
    { 3, 83, 0x1000, 1000 },
    { 3, 84, 0x1000, 1000 },
    { 3, 85, 0x1000, 1000 },
    { 3, 86, 0x1000, 1000 },
    { 3, 87, 0x1000, 1000 },
    { 3, 88, 0x1000, 1000 },
    { 3, 89, 0x1000, 1000 },
    { 3, 90, 0x1000, 1000 },
    { 3, 91, 0x1000, 1000 },
    { 3, 92, 0x1000, 1000 },
    { 3, 93, 0x1000, 1000 },
    { 3, 94, 0x1000, 1000 },
    { 3, 95, 0x1000, 1000 },
    { 3, 96, 0x1000, 1000 },
    { 3, 97, 0x1000, 1000 },
    { 3, 98, 0x1000, 1000 },
    { 3, 99, 0x1000, 1000 },
    { 3, 100, 0x1000, 1000 },
    { 3, 101, 0x1000, 1000 },
    { 3, 102, 0x1000, 1000 },
    { 3, 103, 0x1000, 1000 },
    { 3, 104, 0x1000, 1000 },
    { 3, 105, 0x1000, 1000 },
    { 3, 106, 0x1000, 1000 },
    { 3, 107, 0x1000, 1000 },
    { 3, 108, 0x1000, 1000 },
    { 3, 109, 0x1000, 1000 },
    { 3, 110, 0x1000, 1000 },
    { 3, 111, 0x1000, 1000 },
    { 3, 112, 0x1000, 1000 },
    { 3, 113, 0x1000, 1000 },
    { 3, 114, 0x1000, 1000 },
    { 3, 115, 0x1000, 1000 },
    { 3, 116, 0x1000, 1000 },
    { 3, 117, 0x1000, 1000 },
    { 3, 118, 0x1000, 1000 },
    { 3, 119, 0x1000, 1000 },
    { 3, 120, 0x1000, 1000 },
    { 3, 121, 0x1000, 1000 },
    { 3, 122, 0x1000, 1000 },
    { 3, 123, 0x1000, 1000 },
    { 3, 124, 0x1000, 1000 },
    { 3, 125, 0x1000, 1000 },
    { 3, 126, 0x1000, 1000 },
    { 3, 127, 0x1000, 1000 },
    { 3, 128, 0x1000, 1000 },
    { 3, 129, 0x1000, 1000 },
    { 3, 130, 0x1000, 1000 },
    { 3, 131, 0x1000, 1000 },
    { 3, 132, 0x1000, 1000 },
    { 3, 133, 0x1000, 1000 },
    { 3, 134, 0x1000, 1000 },
    { 3, 135, 0x1000, 1000 },
    { 3, 136, 0x1000, 1000 },
    { 3, 137, 0x1000, 1000 },
    { 3, 138, 0x1000, 1000 },
    { 3, 139, 0x1000, 1000 },
    { 3, 140, 0x1000, 1000 },
    { 3, 141, 0x1000, 1000 },
    { 3, 142, 0x1000, 1000 },
    { 3, 143, 0x1000, 1000 },
    { 3, 144, 0x1000, 1000 },
    { 3, 145, 0x1000, 1000 },
    { 3, 146, 0x1000, 1000 },
    { 3, 147, 0x1000, 1000 },
    { 3, 148, 0x1000, 1000 },
    { 3, 149, 0x1000, 1000 },
    { 3, 150, 0x1000, 1000 },
    { 3, 151, 0x1000, 1000 },
    { 3, 152, 0x1000, 1000 },
    { 3, 153, 0x1000, 1000 },
    { 3, 154, 0x1000, 1000 },
    { 3, 155, 0x1000, 1000 },
    { 3, 156, 0x1000, 1000 },
    { 3, 157, 0x1000, 1000 },
    { 3, 158, 0x1000, 1000 },
    { 3, 159, 0x1000, 1000 },
    { 3, 160, 0x1000, 1000 },
    { 3, 161, 0x1000, 1000 },
    { 3, 162, 0x1000, 1000 },
    { 3, 163, 0x1000, 1000 },
    { 3, 164, 0x1000, 1000 },
    { 3, 165, 0x1000, 1000 },
    { 3, 166, 0x1000, 1000 },
    { 3, 167, 0x1000, 1000 },
    { 3, 168, 0x1000, 1000 },
    { 3, 169, 0x1000, 1000 },
    { 3, 170, 0x1000, 1000 },
    { 3, 171, 0x1000, 1000 },
    { 3, 172, 0x1000, 1000 },
    { 3, 173, 0x1000, 1000 },
    { 3, 174, 0x1000, 1000 },
    { 3, 175, 0x1000, 1000 },
    { 3, 176, 0x1000, 1000 },
    { 3, 177, 0x1000, 1000 },
    { 3, 178, 0x1000, 1000 },
    { 3, 179, 0x1000, 1000 },
    { 3, 180, 0x1000, 1000 },
    { 3, 181, 0x1000, 1000 },
    { 3, 182, 0x1000, 1000 },
    { 3, 183, 0x1000, 1000 },
    { 3, 184, 0x1000, 1000 },
    { 3, 185, 0x1000, 1000 },
    { 3, 186, 0x1000, 1000 },
    { 3, 187, 0x1000, 1000 },
    { 3, 188, 0x1000, 1000 },
    { 3, 189, 0x1000, 1000 },
    { 3, 190, 0x1000, 1000 },
    { 3, 191, 0x1000, 1000 },
    { 3, 192, 0x1000, 1000 },
    { 3, 193, 0x1000, 1000 },
    { 3, 194, 0x1000, 1000 },
    { 3, 195, 0x1000, 1000 },
    { 3, 196, 0x1000, 1000 },
    { 3, 197, 0x1000, 1000 },
    { 3, 198, 0x1000, 1000 },
    { 3, 199, 0x1000, 1000 },
    { 3, 200, 0x1000, 1000 },
    { 3, 201, 0x1000, 1000 },
    { 3, 202, 0x1000, 1000 },
    { 3, 203, 0x1000, 1000 },
    { 3, 204, 0x1000, 1000 },
    { 3, 205, 0x1000, 1000 },
    { 3, 206, 0x1000, 1000 },
    { 3, 207, 0x1000, 1000 },
    { 3, 208, 0x1000, 1000 },
    { 3, 209, 0x1000, 1000 },
    { 3, 210, 0x1000, 1000 },
    { 3, 211, 0x1000, 1000 },
    { 3, 212, 0x1000, 1000 },
    { 3, 213, 0x1000, 1000 },
    { 3, 214, 0x1000, 1000 },
    { 3, 215, 0x1000, 1000 },
    { 3, 216, 0x1000, 1000 },
    { 3, 217, 0x1000, 1000 },
    { 3, 218, 0x1000, 1000 },
    { 3, 219, 0x1000, 1000 },
    { 3, 220, 0x1000, 1000 },
    { 3, 221, 0x1000, 1000 },
    { 3, 222, 0x1000, 1000 },
    { 3, 223, 0x1000, 1000 },
};

dds3ModelFileIndex_t mdlGetNumOfMinorDataEntry5[81] = {
    { 7, 0, 0x1000, 1000 },
    { 7, 1, 0x1000, 1000 },
    { 7, 2, 0x1000, 1000 },
    { 7, 3, 0x1000, 1000 },
    { 7, 4, 0x1000, 1000 },
    { 7, 5, 0x1000, 1000 },
    { 7, 6, 0x1000, 1000 },
    { 7, 7, 0x1000, 1000 },
    { 7, 8, 0x1000, 1000 },
    { 7, 9, 0x1000, 1000 },
    { 7, 10, 0x1000, 1000 },
    { 7, 11, 0x1000, 1000 },
    { 7, 12, 0x1000, 1000 },
    { 7, 13, 0x1000, 1000 },
    { 7, 14, 0x1000, 1000 },
    { 7, 15, 0x1000, 1000 },
    { 7, 16, 0x1000, 1000 },
    { 7, 17, 0x1000, 1000 },
    { 7, 18, 0x1000, 1000 },
    { 7, 19, 0x1000, 1000 },
    { 7, 20, 0x1000, 1000 },
    { 7, 21, 0x1000, 1000 },
    { 7, 22, 0x1000, 1000 },
    { 7, 23, 0x1000, 1000 },
    { 7, 24, 0x1000, 1000 },
    { 7, 25, 0x1000, 1000 },
    { 7, 26, 0x1000, 1000 },
    { 7, 27, 0x1000, 1000 },
    { 7, 28, 0x1000, 1000 },
    { 7, 29, 0x1000, 1000 },
    { 7, 30, 0x1000, 1000 },
    { 7, 31, 0x1000, 1000 },
    { 7, 32, 0x1000, 1000 },
    { 7, 33, 0x1000, 1000 },
    { 7, 34, 0x1000, 1000 },
    { 7, 35, 0x1000, 1000 },
    { 7, 36, 0x1000, 1000 },
    { 7, 37, 0x1000, 1000 },
    { 7, 38, 0x1000, 1000 },
    { 7, 39, 0x1000, 1000 },
    { 7, 40, 0x1000, 1000 },
    { 7, 41, 0x1000, 1000 },
    { 7, 42, 0x1000, 1000 },
    { 7, 43, 0x1000, 1000 },
    { 7, 44, 0x1000, 1000 },
    { 7, 45, 0x1000, 1000 },
    { 7, 46, 0x1000, 1000 },
    { 7, 47, 0x1000, 1000 },
    { 7, 48, 0x1000, 1000 },
    { 7, 49, 0x1000, 1000 },
    { 7, 50, 0x1000, 1000 },
    { 7, 51, 0x1000, 1000 },
    { 7, 52, 0x1000, 1000 },
    { 7, 53, 0x1000, 1000 },
    { 7, 54, 0x1000, 1000 },
    { 7, 55, 0x1000, 1000 },
    { 7, 56, 0x1000, 1000 },
    { 7, 57, 0x1000, 1000 },
    { 7, 58, 0x1000, 1000 },
    { 7, 59, 0x1000, 1000 },
    { 7, 60, 0x1000, 1000 },
    { 7, 61, 0x1000, 1000 },
    { 7, 62, 0x1000, 1000 },
    { 7, 63, 0x1000, 1000 },
    { 7, 64, 0x1000, 1000 },
    { 7, 65, 0x1000, 1000 },
    { 7, 66, 0x1000, 1000 },
    { 7, 67, 0x1000, 1000 },
    { 7, 68, 0x1000, 1000 },
    { 7, 69, 0x1000, 1000 },
    { 7, 70, 0x1000, 1000 },
    { 7, 71, 0x1000, 1000 },
    { 7, 72, 0x1000, 1000 },
    { 7, 73, 0x1000, 1000 },
    { 7, 74, 0x1000, 1000 },
    { 7, 75, 0x1000, 1000 },
    { 7, 76, 0x1000, 1000 },
    { 7, 77, 0x1000, 1000 },
    { 7, 78, 0x1000, 1000 },
    { 7, 79, 0x1000, 1000 },
    { 7, 80, 0x1000, 1000 },
};

dds3ModelFileIndex_t mdlGetNumOfMinorDataEntry6[13] = {
    { 4, 0, 0x1000, 1000 },
    { 4, 1, 0x1000, 1000 },
    { 4, 2, 0x1000, 1000 },
    { 4, 3, 0x1000, 1000 },
    { 4, 4, 0x1000, 1000 },
    { 4, 5, 0x1000, 1000 },
    { 4, 6, 0x1000, 1000 },
    { 4, 7, 0x1000, 1000 },
    { 4, 8, 0x1000, 1000 },
    { 4, 9, 0x1000, 1000 },
    { 4, 10, 0x1000, 1000 },
    { 4, 11, 0x1000, 1000 },
    { 4, 12, 0x1000, 1000 },
};

mdlFileKindSlot_t modelFileKindTable[8] = {
    { modelFileKindTableEntry, 34 },
    { mdlGetNumOfMinorDataEntry, 352 },
    { mdlGetNumOfMinorDataEntry3, 80 },
    { mdlGetNumOfMinorDataEntry4, 224 },
    { mdlGetNumOfMinorDataEntry6, 13 },
    { NULL, 0 },
    { mdlGetNumOfMinorDataEntry2, 80 },
    { mdlGetNumOfMinorDataEntry5, 81 },
};
#endif
