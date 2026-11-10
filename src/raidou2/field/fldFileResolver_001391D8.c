#include "common.h"

/* Declarations from smt3-decomp src/smt3/field/fldFileResolver.c */
#include "common.h"

#include "sdf_resource.h"

#include "basic.h"

#include "dds3obj.h"

#include "field.h"

#include "fileManager.h"

enum fldPackedResourceKind
{
    FLD_PACKED_RESOURCE_INFO_TABLE = 1,
    FLD_PACKED_RESOURCE_NPC_PALETTE = 2,
    FLD_PACKED_RESOURCE_SKY_BUFFER = 3,
    FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE = 4,
    FLD_PACKED_RESOURCE_RETAINED_COPY = 5,
    FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK = 6
};

typedef struct objectTransform_t objectTransform_t;

typedef s32 (*dds3MoverUpdate_t)(struct objectTransform_t *, dds3Basic_t *);

typedef struct worldIndexNode_t worldIndexNode_t;

typedef struct motion_t motion_t;

typedef struct fldMoverWork_t fldMoverWork_t;

typedef struct fldTransferChunk_t fldTransferChunk_t;

typedef struct fldNpcPalette_t fldNpcPalette_t;

typedef struct fldSkyBuffer_t fldSkyBuffer_t;

/* Kind 3 allocates 0x10 bytes in dds3AllocateClearedObjectWork. */
struct fldMoverWork_t {
    dds3Basic_t *target; /* 0x00: object whose transform is updated. */
    dds3PathBasicControl_t *path; /* 0x04: retained curve work. */
    dds3MoverUpdate_t update; /* 0x08: used when no curve work is retained. */
    dds3Basic_t *sourceObject; /* 0x0C: kind-16 curve source. */
};

/* Packed-resource transfer descriptor; the first word's role is unknown. */
struct fldTransferChunk_t {
    u32 unk00; /* 0x00: opaque descriptor word */
    s32 offset; /* 0x04: signed byte offset from the resource base */
    u32 size; /* 0x08: byte length of the transferred payload */
};

dds3Basic_t *fldMakeDmyMatter(void);

extern dds3Basic_t *dds3AddMatterObjectBasic(s32 serialID, f32 *pos, f32 *rot);

extern u32 dds3AllocObjectId(void);

extern void dds3SetUnitObjectId(dds3Basic_t *node, addr_t value);

dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 modelRangeID);

/* Return the borrowed secondary world node, or NULL when no world is active. */
dds3Basic_t *dds3GetControlScene(void);

extern fldResHead_t *g_pHitData;

extern u32 g_nHitData;

extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const u8 *name);

/* Read or update the cached world object associated with a resource record ID. */
dds3Basic_t *fldFindMatterIndex(s32 id);

extern void fldFindMatterIndexFrom(s32 id, dds3Basic_t *worldNode);

dds3Basic_t *fldGetModelRange_From_MatterName(const char *matterName);

extern s32 strcmp(const char *left, const char *right);

void fldMakeFilePath(char *pathstr, s32 field, s32 area);

#include "sdk_libc.h"

s32 fldMakeObjectBasic3(void);

void fldMakeObjectBasic2(void);

typedef struct fldModelData_t {
    u8 unk00[16]; /* 0x00 */
    u8 modelDef[16]; /* 0x10: the model definition dds3AddModelBasic2 takes */
    void *playBook; /* 0x20: motion play book */
} fldModelData_t;

typedef struct fldResourceAreaRef_t {
    u32 word00; /* 0x00 */
    const u8 *name; /* 0x04 */
} fldResourceAreaRef_t;

extern dds3Basic_t *dds3AddModelBasic2(u32 serialID, void *pModelDef, void *pMotDef, s32 pTexList, const char *pName);

extern void dds3MatterObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx);

extern s32 fldCommand_GetHojiID(const char *pName);

extern void dds3MatterObjectBasicSetHojiID(dds3Basic_t *aPtr, s32 hojiID);

extern dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic);

extern void dds3ObjectBaseMakeMatter(dds3Basic_t *pObjectBasic);

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *pObjectBasic);

extern fldResHead_t *fldScriptResource;

extern u32 fldScriptResourceCount;

extern s32 fldGetHojiFlag(s32 mapId, u32 slotIndex, s32 bit);

extern s32 fldCommand_SEARCH_ID(const char *);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

extern void dds3ObjectBaseCreateMover(void *pObjectBasic);

extern void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);

extern void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);

extern dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *pMoverBasic);

extern void evtPathRate(dds3Basic_t *basic, f32 rate);

extern void dds3MoverBasicUpdate(dds3Basic_t *);

extern s32 fldGetModelFlag(s32 mapId, u32 slotIndex, s32 bit);

extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *, u32);

u8 *fldGetInfoHeader(fldTransferChunk_t *pData);

void sdfAddressResolverReset(u32 *aDataBody, u32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng);

void fldAddressResolver(addr_t pFileTop, fldTransferChunk_t *pData);

extern void sdfAddressResolver(s32 *aDataBody, saddr_t aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng);

void fldAddressResolverReset(addr_t pFileTop, fldTransferChunk_t *pData);

void fldLoadNextFile_SetId_BtlRet(u32 field, u32 area);

extern u32 fldBtlNextField;

extern u32 fldBtlNextArea;

s32 fldLoadNextFile_BtlRet(void);

void fldFreeNextFile(void);

extern fileRequest_t *pFldNextFileDataF2;

fileRequest_t *pacLoad(const char *filename);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

extern s32 pacFileHandleFree_(void *pacHandle);

u8 fldLoadNextGetStatus2(void);

extern s32 pacCheckEnd(const fileRequest_t *pacHandle);

s32 fldLoadNextGetStatus3(s32 field, s32 area);

sdfMemHandle_t *fldLoadNextFileTB(void **pData, s32 field, s32 area);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern u32 sdfMemLock(sdfMemHandle_t *aHandle);

extern void *memcpy(void *a, const void *b, size_t c);

sdfMemHandle_t *fldLoadNextFileF1(void **pData, s32 field, s32 area);

sdfMemHandle_t *fldLoadNextFileF2(void **pData, s32 field, s32 area);

extern u32 fldNextTbSize, fldNextF1Size, fldNextF1Size, fldNextF2Size;

extern u32 fldNextTbData, fldNextF1Data, fldNextF1Data, fldNextF2Data;

u8 fldCheckNextFile(void);

void fldPreloadNextFile(char *out);

extern s32 evtBitCheck(s32 no);

extern char fldFieldPackFlag[];

void fldLoadFieldPack(void);

extern char * strcpy(char *, const char *);

extern fileRequest_t *gFldFileF1;

extern void pacSync(fileRequest_t *pacFileHandle);

extern void fldInfLoadFromMem(const void *);

extern void fldGetEAEntry(const fldNpcPalette_t *source);

extern void fldGetSkyEntry(fldSkyBuffer_t *source);

extern void fldGetDoorEntry(const void *source);

extern sdfMemHandle_t *fldFieldPackData;

extern s32 sdfMemGetSize(const sdfMemHandle_t *aHandle);

extern u8 *sdfMemGetPtr(const sdfMemHandle_t *aHandle);

extern void fldAmbLoadFromMem(u8 *data, saddr_t mh);

extern void fldAutoMapFontInit(s32 bank);

void fldFreePac0(void);

void fldFreeFile(void);

u32 fldLoadFile(const char *name);

extern u32 gFldInf_WpExit;

extern s32 dds3GetMapNo(dds3Basic_t *scene);

extern void fldPlayerALoad(void);

extern void fldInitBlurEffect(void);

extern s32 sdfDevLoadTexList(const char *aFilename);

extern void fldSetupDoorScenes(void);

extern s32 dds3SceneBasicLoadScriptFile_FromMem(dds3Basic_t *scene, void *mem);

extern volatile s32 fldSceneParam[];

extern char fldFmtTbPath[];

extern char fldFmtF2Path[];

extern char fldFmtF1Path[];

extern s32 dds3SearchProcessName(const char *namestr);

extern void fldDrwZFree(void);

extern void fldClearSe3D(void);

extern s32 fldTitleChk(void);

extern s32 fldTitleMiniChk(void);

extern s32 dds3KillProcessName(const char *name, s32 shutdowntimer);

extern void fldFreeModelTable(void);

extern void fldClearTakaraSlots(void);

extern void fldFreeNpcModel(void);

extern void fldAutoMapFree(void);

extern void fldAutoMapFileFree(void);

extern void fldFreeBlurEffect(void);

extern void fldClearMatterFlags(void);

extern void fldClearEveRngTable(void);

extern void fldSe_Init(void);

extern void fldClearModelSlots(void);

extern s32 sdfTexListFree(s32 aList, s32 aReleaseItems);

extern sdfMemHandle_t *fldD3pHandle[];

extern void *fldD3pData[];

extern addr_t fldScriptBufHandle;

extern fileRequest_t *fldFileBuf1;

extern u8 D_0054EC00[];

void fldFileAddrResetMode(u32 mode);

void fldHojiDebugView(void);

void fldHojiDebugView2(void);

extern s32 fldGetAnimeFlag(s32 field, s32 area, s32 block);

extern void fldDbgPrint2D(s32 x, s32 y, const char *str);

extern s32 fldGbChkHit(s32 fld, s32 area, s32 blk);

extern char fldFmtRowLabel[];

extern char fldFmtDot[];

extern char fldFmtInt[];

extern u32 gFldFileResetMode;

typedef struct fldFileAreaDataFL1_t {
    u32 fileID;
    u32 SkyTblIdx;
    char *filename0;
    char *filename1;
    char *filename2;
    char *filename3;
} fldFileAreaDataFL1_t;

typedef struct fldFileInfoHeaderFL1_t {
    u32 nResourceTypeTable;
    void *pResourceTypeTable;
    fldFileAreaDataFL1_t *pAreaData;
} fldFileInfoHeaderFL1_t;

extern void fldFileResolverFL1(fldFileInfoHeaderFL1_t *info);

extern void fldSetFileResolverIdx(u32 index);

extern char *fldResolverFL1Data[4];

extern char fldFmtResPath[];

extern void *memset(void *buffer, s32 value, size_t size);

extern void fldMakeResouceBasicFL1(void *typeTable, u32 typeCount);

u32 fldLoadFile_FromMemory(s32 field, s32 area, u8 *fl1Buffer, u8 *fl2Buffer, void *textureBuffer);

extern void fldDrwZAlloc(void);

extern void fldAutoMapSetField(s32 fld, s32 area, s32 block);

extern s32 sdfTexSetTB(void *aData);

extern u32 fldTexList;

extern addr_t fldNextTbHandle, fldFileBufF1, fldFileBuf2;

extern u8 *FildMaskObj, *mat_black, *mat_white, *mat_line;

typedef struct fldConstruntAreaFL2_t {
    u32 flag; /* 0x00 */
    const char *areaName; /* 0x04 */
    u32 areaID; /* 0x08 */
} fldConstruntAreaFL2_t;

extern dds3Basic_t *dds3AddAreaBasic(u32 serialID, const char *pName);

void fldMakeBlockBasicFL2(fldConstruntAreaFL2_t *pBlockTable, u32 nBlock);

s32 fldLoadNextFile(s32 field, s32 area);

typedef struct fldFileAreaDataFL2_t {
    u32 word00;
    s32 label; /* 0x04: the script entry recorded for a field retry. */
} fldFileAreaDataFL2_t;

typedef struct fldFileInfoHeaderFL2_t {
    u32 nResourceTypeTable; /* 0x00 */
    void *pResourceTypeTable; /* 0x04 */
    fldFileAreaDataFL2_t *pAreaData; /* 0x08 */
    u32 nArea; /* 0x0C */
    fldConstruntAreaFL2_t *pAreaTable; /* 0x10 */
} fldFileInfoHeaderFL2_t;

extern void fldMakeResouceBasicFL2(void *typeTable, u32 typeCount);

void fldFileResolverFL2(fldFileInfoHeaderFL2_t *info);

s32 fldLoadNextTask(void);

dds3Basic_t *fldMakeDmyMatter(void);

dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 modelRangeID);

dds3Basic_t *fldGetModelRange_From_MatterName(const char *matterName);

/* FL1 and FL2 use the same 0x0C-byte resource-type table entries. */
typedef struct fldSpawnBatch_t {
    u32 kind;
    u32 count;
    fldResHead_t *entries;
} fldSpawnBatch_t;

typedef struct fldBillConfigView_t {
    u32 flags;
    u32 kind;
    u32 resourceIndex;
    f32 width;
} fldBillConfigView_t;

typedef struct fldLightDataView_t {
    u8 pad00[0x64];
    u32 flags;
} fldLightDataView_t;

typedef struct fldCollisionDescriptor_t fldCollisionDescriptor_t;

typedef struct fldSpawnCollisionView_t {
    u32 kind;
    u32 word04;
    fldCollisionDescriptor_t *header;
} fldSpawnCollisionView_t;

enum {
    FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE = 2,
    FLD_SPAWN_BATCH_KIND_COLLISION = 3,
    FLD_SPAWN_BATCH_KIND_CAMERA = 4,
    FLD_SPAWN_BATCH_KIND_EVENT = 6,
    FLD_SPAWN_BATCH_KIND_UNSUPPORTED = 7,
    FLD_SPAWN_BATCH_KIND_PATH = 9,
    FLD_SPAWN_BATCH_KIND_POINT = 10,
    FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT = 11,
    FLD_SPAWN_BATCH_KIND_LIGHT = 12
};

typedef struct fldResourceEffect_t fldResourceEffect_t;

typedef struct fldResourceLight_t {
    u32 resv, Flag, AnimeType;
    f32 InsideR, OutsideR, Soften, Bias;
    f32 DiffuseR, DiffuseG, DiffuseB;
    f32 AmbientR, AmbientG, AmbientB;
} fldResourceLight_t;

extern dds3Basic_t *dds3AddTMXEffectObjectBasic(const char *, void *, saddr_t);

extern dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *, void *, saddr_t);

extern dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *, void *, saddr_t);

extern dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *, void *, saddr_t);

extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *, void *, void *);

extern dds3Basic_t *dds3AddESPEffectObjectBasic(const char *, void *, void *, f32);

extern void dds3EffectObjectBasicStatOn(dds3Basic_t *, s32);

extern void dds3EffectObjectBasicStatOff(dds3Basic_t *, s32);

extern void dds3EffectObjectBasicSetFldEx(dds3Basic_t *, fldResourceEffect_t *);

extern dds3Basic_t *dds3AddLightObjectBasic(s32);

extern void dds3GetRadiusPoseSet(dds3Basic_t *, f32);

extern void dds3SetLightObjectResoruce(dds3Basic_t *, fldResourceLight_t *);

extern dds3Basic_t *dds3AddPathBasic(s32, void *, const char *);

extern void fldMakePlane(fldCollisionDescriptor_t *, f32 *, u32, s32, s32);

extern dds3Basic_t *dds3AddModelRangeBasicNew(s32, void *, f32 *, const char *);

extern void dds3SetPathBasicSpline(dds3Basic_t *, u32);

void fldMakeResouceBasicFL1(void *typeTable, u32 batchCount);

/* Resolved FL2 event and placement payloads, selected by the batch kind. */
typedef struct fldSpawnEventDataFL2_t {
    u32 kind;
    void *data;
} fldSpawnEventDataFL2_t;

typedef struct fldSpawnSoundDataFL2_t {
    f32 radius;
    s32 soundId;
} fldSpawnSoundDataFL2_t;

typedef struct fldEventDataHeader_t fldEventDataHeader_t;

extern dds3Basic_t *dds3AddEventBasic(s32 serialID, void *data, const char *name);

extern u32 fldEveHit_AddEveData(fldEventDataHeader_t *resource, dds3Basic_t *basic);

extern s32 fldEveHit_AddEveHit(fldResHead_t *resource, dds3Basic_t *basic);

extern dds3Basic_t *dds3AddPointBasic(u32 serialID, fldResourcePointUnaligned_t *data, const char *name);

extern void fldNpcLoad(s32 npcId, f32 x, f32 y, f32 z, const f32 *rotation, s32 room, const char *name, s32 eventIndex);

extern void fldItemBoxAdd(s32 itemId, const sdfVector4_t *rotation, f32 x, f32 y, f32 z);

extern void fldAddSe3D(s32 soundId, f32 radius, f32 x, f32 y, f32 z);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(u32 serialID, const sdfVector4_t *position, const sdfVector4_t *rotation);

void fldMakeResouceBasicFL2(void *typeTable, u32 batchCount);

void fldMakeBlockBasicFL2(fldConstruntAreaFL2_t *pBlockTable, u32 nBlock);

void fldMakeObjectBasic2(void);

/* Restore the room's named objects and enable their retained path controllers. */
s32 fldMakeObjectBasic3(void);

void fldFileResolverFL1(fldFileInfoHeaderFL1_t *info);

void fldFileResolverFL2(fldFileInfoHeaderFL2_t *info);

u8 *fldGetInfoHeader(fldTransferChunk_t *pData);

void sdfAddressResolverReset(u32 *aDataBody, u32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng);

/* Relocate the words described by this packed-resource transfer chunk. */
void fldAddressResolver(addr_t pFileTop, fldTransferChunk_t *pData);

void fldAddressResolverReset(addr_t pFileTop, fldTransferChunk_t *pData);

void fldLoadNextFile_SetId_BtlRet(u32 field, u32 area);

s32 fldLoadNextFile_BtlRet(void);

s32 fldLoadNextFile(s32 field, s32 area);

void fldFreeNextFile(void);

s32 fldLoadNextTask(void);

u32 fldLoadNextGetStatus(void);

u8 fldLoadNextGetStatus2(void);

s32 fldLoadNextGetStatus3(s32 field, s32 area);

sdfMemHandle_t *fldLoadNextFileTB(void **pData, s32 field, s32 area);

sdfMemHandle_t *fldLoadNextFileF1(void **pData, s32 field, s32 area);

sdfMemHandle_t *fldLoadNextFileF2(void **pData, s32 field, s32 area);

void fldPreloadNextFile(char *out);

u8 fldCheckNextFile(void);

void fldLoadFieldPack(void);

void fldFreePac0(void);

u32 fldLoadFile(const char *name);

u32 fldLoadFile_FromMemory(s32 field, s32 area, u8 *fl1Buffer, u8 *fl2Buffer, void *textureBuffer);

void fldFreeFile(void);

/* Select the ordinary field, battle or event resource directory. */
void fldMakeFilePath(char *pathstr, s32 field, s32 area);

void fldHojiDebugView(void);

void fldHojiDebugView2(void);

/* Enable or skip relocation of remaining transfer chunks during field teardown. */
void fldFileAddrResetMode(u32 mode);

extern u32 fldTexList;

extern u8 *FildMaskObj;

extern u8 *mat_black;

extern u8 *mat_white;

extern u8 *mat_line;

extern u32 gFldFileResetMode;

extern fileRequest_t *gFldFileF1;

extern fileRequest_t *fldFileBuf1;

extern fileRequest_t *pFldNextFileDataF2;

extern char fldFmtResPath[8];

extern sdfMemHandle_t *fldFieldPackData;

extern addr_t fldScriptBufHandle;

extern addr_t fldNextTbHandle;

extern addr_t fldFileBufF1;

extern addr_t fldFileBuf2;

extern u32 fldNextTbData;

extern u32 fldNextF1Data;

extern u32 fldNextF2Data;

extern u32 fldNextTbSize;

extern u32 fldNextF1Size;

extern u32 fldNextF2Size;

extern u32 fldBtlNextField;

extern u32 fldBtlNextArea;

extern u8 D_0054EC00[1];

extern char fldFmtTbPath[8];

extern char fldFmtF2Path[8];

extern char fldFmtF1Path[8];

extern char fldFmtRowLabel[8];

extern char fldFmtDot[8];

extern char fldFmtInt[8];

extern char D_0054EC38[8];

extern s32 rsetFrameMaskOtOffset;
/* End of declarations from src/smt3/field/fldFileResolver.c */

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", fldMakeBlockBasicFL2);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139258);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139460);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_001394E8);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139520);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139540);

void sdfAddressResolverReset(u32 *aDataBody, u32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng) {
    const u8 *p = aAddrInfoPtr;
    s32 code;
    s32 i;

    while (p - aAddrInfoPtr < aAddrInfoLeng) {
        code = *p++;
        if ((code & 1) == 0) {
            code >>= 1;
        } else if ((code & 2) == 0) {
            code = (code | (*p++ << 8)) >> 2;
        } else if ((code & 4) == 0) {
            code = (code | (p[0] << 8) | (p[1] << 16)) >> 3;
            p += 2;
        } else {
            code = (code >> 3) + 2;
            for (i = 0; i < code; i++) {
                aDataBody++;
                *aDataBody -= aOffset;
            }
            continue;
        }
        aDataBody += code;
        *aDataBody -= aOffset;
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139630);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139658);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139680);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139688);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139690);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139698);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_001396A0);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_001396A8);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_001396B0);

void func_001396B8(void) {
}

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_001396C0);

INCLUDE_ASM(const s32, "field/fldFileResolver_001391D8", func_00139730);
INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE85C);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE860);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE864);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE868);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE86C);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE870);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE874);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE878);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE87C);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE880);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE884);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE888);

INCLUDE_SDATA(const s32, "field/fldFileResolver_001391D8", D_004DE88C);

