/* Declarations of smt3-decomp src/smt3/model/mdlManager.c, for its bodies in this directory. */
#ifndef SHARED_MODEL_MDLMANAGER_H
#define SHARED_MODEL_MDLMANAGER_H

#include "common.h"

#include "model.h"

#include "motion.h"

#include "draw3d.h"

#include "ps2asm.h"

#include "sdfFrame.h"

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

extern void *sdfChipAllocDirect(u32 aSize);

extern void *sdfChipAllocZeroDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

extern void sdfTexListFree(sdfModelTexList_t *aList, s32 aReleaseItems);

extern sdfModelTexList_t *sdfTexListDup(sdfModelTexList_t *aList);

extern sdfModelRootNode_t *sdfModelMake(sdfModelTexList_t *aTexList, sdfModelDef_t *aModelData);

extern void sdfModelRootFree(sdfModelRootNode_t *aRoot, s32 aReleaseNodes, s32 aReleaseMatList);

extern sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *aModel,
                                           sdfMotionPlayBook_t *aBook);

extern void sdfMotionCtrlrFree(sdfMotionCtrlr_t *aCtrlr);

extern void sdfMotionPause(sdfMotionCtrlr_t *aCtrlr);

extern void sdfMotionPauseOff(sdfMotionCtrlr_t *aCtrlr);

extern void fileManager(void);

extern void mdlSetEffectColorSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, u32 aColor);

extern void mdlSetEffectScaleSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, f32 aScale);

extern void ToMatrix(void);

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

extern sdfModelTexList_t *sdfTexSetTB(void *aData);

extern sdfModelDef_t *sdfFixupModelBinary(void *aData);

extern sdfMotionPlayBook_t *sdfFixupMotionBinary(void *aData);

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

extern char *strcpy(char *dst, const char *src);

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

extern void sdfMotionNext(sdfMotionCtrlr_t *aCtrlr);

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

extern mdlFileSlot_t modelFileTable[];

extern mdlFileKindSlot_t modelFileKindTable[];

extern dds3ModelFileIndex_t *mdlHddFileIndex;

extern dds3ModelFileTable_t *mdlHddFileList;

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern s32 strlen(const char *s);

void mdlReleaseHddFileList(void);

extern sdfMemHandle_t *mdlHddFileIndexHandle;

extern sdfMemHandle_t *mdlHddFileListHandle;

extern void sdfMemFreeHandleDirect(sdfMemHandle_t **aHandlePtr);

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

extern const mdlPathPrefix_t mdlModelPathPrefix[];

extern const char mdlPackedBundleExt[];

extern const mdlWorkPrefix_t mdlWorkPathPrefix[];

extern mdlHddFileEntry_t *mdlHddFileCount;

extern u8 sdfHddValid;

extern const char _ctype_[];

extern s32 sceDopen(const char *name);

extern s32 sceDread(s32 handle, mdlDirectoryEntry_t *entry);

extern s32 sceDclose(s32 handle);

extern s32 memcmp(const void *first, const void *second, size_t size);

extern void *memcpy(void *a, const void *b, size_t c);

extern void *memset(void *a, s32 b, size_t c);

extern char *strcat(char *dst, const char *src);

extern char *strrchr(const char *text, s32 character);

extern s32 strcasecmp(const char *first, const char *second);

extern void sdfDummyFunc(void);

extern fileRequest_t *fileLoadEx(const char *filename, s32 headOrTail, addr_t endCallback, addr_t callbackParm);

extern void sdfModelCopyStat(sdfModelRootNode_t *aDstRoot, sdfModelRootNode_t *aSrcRoot);

extern void sdfMotionChangeBlend(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType, f32 aPreFrame, f32 aBlendLeng);

extern void mdlStopEffect(mdlObj_t *aHandle, s32 aPlayGroup);

extern u16 mdlPibGetEffectCount(mdlPibRecord_t *record);

extern u16 mdlPibGetMotionCount(mdlPibRecord_t *record);

extern s32 mdlPibGetVersion(mdlPibRecord_t *record);

extern sdfPacDecodeResrc_t *sdfPacFreeResrc(sdfPacDecodeResrc_t *resrc);

extern sdfMatrix4_t D_002FA240;

extern sdf3DScreenParam_t dds3ScreenParam;

extern void MultMatrixRev(sdfMatrix4_t *aMatrix);

extern void MultMatrixRevNoArg(void);

s32 mdlFileLoad(s32 major, s32 minor, s32 regFlags, const char *filename, void (*callback)(u32), u32 parm);

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

dds3ModelResrc_t *mdlSearchResrc(s32 aMajor, s32 aMinor);

s32 mdlCheckLoading(s32 aMajor, s32 aMinor);

void mdlRegLoading(s32 aMajor, s32 aMinor);

void mdlUnRegLoading(s32 aMajor, s32 aMinor);

void mdlRegist(s32 aMajor, s32 aMinor, s32 aFlag,
               sdfModelTexList_t *aTexList, sdfModelDef_t *aData,
               sdfMemHandle_t *aHandle);

void mdlUnRegistResrc(dds3ModelResrc_t *resrc_p);

void mdlUnRegist(s32 aMajor, s32 aMinor);

void mdlUnRegistAll(void);

sdfMotionCtrlr_t *mdlCreateMotion(mdlObj_t *aHandle, s32 aGroup);

void mdlFreeMotionCtrlr(mdlObj_t *aHandle, sdfMotionCtrlr_t *aCtrlr);

void mdlFreeMotion(mdlObj_t *aHandle, s32 aGroup);

void mdlUnRegistResrcMotion(dds3ModelResrc_t *aResrc, s32 aGroup);

void mdlUnRegistMotion(s32 aMajor, s32 aMinor, s32 aGroup);

void mdlRegistMotion(s32 aMajor, s32 aMinor, u32 aFlags, s32 aFirstMotion, s32 aGroup, s32 aPlayGroup,
                     sdfMotionPlayBook_t *aData, sdfMemHandle_t *aHandle);

void mdlRegistModelSet(s32 aMajor, s32 aMinor, s32 aFlag, mdlSetDef_t *aSet);

dds3ModelResrc_t *mdlWaitLoadEnd(s32 aMajor, s32 aMinor);

void TbMbAbLoadEnd(mdlManager_mdlTbMbAbLoadCb_t *tmaCb);

void TbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm);

void MbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm);

void AbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm);

char *mdlSetFileName(char *buff, const char *fname);

saddr_t mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);

dds3ModelResrc_t *mdlLoadPrepare(u32 group, u32 id);

void mdlUnlinkResrc(mdlObj_t *aHandle);

void mdlSetBlur(mdlObj_t *aHandle, s32 interval, s32 count,
                         u32 startColor, u32 endColor);

void mdlFreeBlur(mdlObj_t *aHandle);

void mdlLinkBlur(mdlObj_t *ctx, struct sdfGrOt_t **surfaces);

mdlObj_t *mdlLoad(u32 aMajor, u32 aMinor);

void mdlFree(mdlObj_t *aHandle);

void mdlLink(mdlObj_t *aHandle, void *aModelOt);

void mdlHideAllNode(mdlObj_t *aHandle);

void mdlMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, s32 aType,
                                     f32 aBeforeLeng, f32 aSmoothLeng);

void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                         f32 aSmoothLeng);

void mdlSmoothSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                           f32 aSmoothLeng);

sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *aHandle, s32 aGroup);

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

void mdlSetEffectColor(mdlObj_t *aHandle, u32 aColor);

void mdlSetColor(mdlObj_t *aHandle, u32 aColor);

void mdlSetColorWithEffect(mdlObj_t *aHandle, u32 aColor);

void mdlSetEffectScale(mdlObj_t *aHandle, f32 aScale);

/* vu0 routine: project the point through the view and model matrices into vf10. */
void mdlGetScreenOffset(mdlObj_t *aHandle, void *aOfs);

/* vu0 routine: project an array of points and store their screen coordinates. */
void mdl_1DD9C0(mdlObj_t *ctx, f32 (*inputPoints)[4], f32 (*outputPoints)[4], s32 pointCount);

void mdlMotionPause(mdlObj_t *aHandle);

void mdlMotionPauseOff(mdlObj_t *aHandle);

s32 mdlHasMotionEx(mdlObj_t *aHandle, s32 aGroup);

s32 mdlGetMajorNum(mdlObj_t *aHandle);

s32 mdlGetMinorNum(mdlObj_t *aHandle);

s32 mdlGetNumOfMajor(void);

s32 mdlGetNumOfMinor(s32 aMajor);

s32 mdlGetNumOfCut(mdlObj_t *aHandle, s32 aGroup);

void mdlSetClutColor(mdlObj_t *ctx, s32 color, f32 ratio);

void mdlGetFilebase(s32 aMajor, s32 aMinor, char *aName, s32 aNameLeng);

s32 mdlPackCheck(sdfPacDecodeResrc_t *resrc);

sdfPacDecodeResrc_t *mdlPackSetOld(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags);

sdfPacDecodeResrc_t *mdlPackSet(sdfPacDecodeResrc_t *resrc, s32 major, s32 minor, s32 regFlags);

void mdlFileCbFree(mdlManager_mdlFileLoadCb_t *cb);

void mdlFileLoadCallback(filePacRequest_t *pacHandle, mdlManager_mdlFileLoadCb_t *data);

s32 mdlFileLoad(s32 major, s32 minor, s32 regFlags, const char *filename, void (*callback)(u32),
                u32 parm);

void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);

void mdlFreeFileItem(void *aItem);

void mdlReleaseHddFileList(void);

void mdlMakeHddFileListDummy(void);

void mdlMakeHddFileList(void);

#endif
