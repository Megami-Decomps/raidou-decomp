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

/* Declarations from dds-decomp src/dds1/field/fldFileResolver.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_INFO_TABLE 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_NPC_PALETTE 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_RETAINED_COPY 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_SKY_BUFFER 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_CAMERA 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_COLLISION 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_EVENT 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_LIGHT 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_PATH 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_POINT 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_UNSUPPORTED 1

#define SMT3_DEFINES_dds3MoverUpdate_t 1

#define SMT3_DEFINES_fldBillConfigView_t 1

#define SMT3_DEFINES_fldCollisionDescriptor_t 1

#define SMT3_DEFINES_fldConstruntAreaFL2_t 1

#define SMT3_DEFINES_fldEventDataHeader_t 1

#define SMT3_DEFINES_fldFileAreaDataFL1_t 1

#define SMT3_DEFINES_fldFileAreaDataFL2_t 1

#define SMT3_DEFINES_fldFileInfoHeaderFL1_t 1

#define SMT3_DEFINES_fldFileInfoHeaderFL2_t 1

#define SMT3_DEFINES_fldLightDataView_t 1

#define SMT3_DEFINES_fldModelData_t 1

#define SMT3_DEFINES_fldMoverWork_t 1

#define SMT3_DEFINES_fldNpcPalette_t 1

#define SMT3_DEFINES_fldPackedResourceKind 1

#define SMT3_DEFINES_fldResourceAreaRef_t 1

#define SMT3_DEFINES_fldResourceEffect_t 1

#define SMT3_DEFINES_fldResourceLight_t 1

#define SMT3_DEFINES_fldSkyBuffer_t 1

#define SMT3_DEFINES_fldSpawnBatch_t 1

#define SMT3_DEFINES_fldSpawnCollisionView_t 1

#define SMT3_DEFINES_fldSpawnEventDataFL2_t 1

#define SMT3_DEFINES_fldSpawnSoundDataFL2_t 1

#define SMT3_DEFINES_fldTransferChunk_t 1

#define SMT3_DEFINES_motion_t 1

#define SMT3_DEFINES_objectTransform_t 1

#define SMT3_DEFINES_worldIndexNode_t 1

#include "kernel_draw.h"

#include "sdf_packet_list.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_fade.h"

#include "dds/dds3_object_flags.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_world_primary.h"

#include "dds/dds3obj.h"

#include "dds/eff_transform.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/evt_world.h"

#include "dds/fld_scene_ready_api.h"

#include "dds/fr_font.h"

#include "dds/mdl.h"

#include "dds/mdl_motion_api.h"

#include "dds/sdf.h"

#include "dds/sdf_column_packet_api.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_gs_blend.h"

#include "dds/sdf_gs_packet.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/sdf_map_position_api.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_header.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/sdf_sif_command.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_texture_primary_buffer_api.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

/* Load matrix into the VU0 scratch bank and postmultiply the primary bank. */
void MultMatrixRev(const struct Matrix4 *matrix);

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};

enum SdfMotionStateValue {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
};

/* Load a Y-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixZ(f32 angle);

void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);

void dds3FadeIn2(s32 duration);

struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *world);

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

/* Return the current frame draw-buffer slot (0 or 1). */
u32 dds3GetFrameID(void);

void dds3GetPosPoseSet(dds3Basic_t *object);

void dds3GetRotPoseSet(dds3Basic_t *object);

void dds3ObjectBaseOffFlag(struct dds3Basic_t *object, u32 mask);

/* Set or clear requested bits in the object's flag word. */
void dds3ObjectBaseOnFlag(struct dds3Basic_t *object, u32 mask);

void dds3ObjectBaseSetControlBasicSvs(dds3Basic_t *object, u32 value);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* Preserve the signed-byte drawing gate; only a missing payload is ignored. */
void dds3VisibleSceneBasic(dds3Basic_t *object, s8 enabled);

struct SdfPacket *effDebugDrawWireModel(const f32 vertices[][4], const u32 *colors,
                                                 s32 vertexCount, u32 primitiveFlags);

evtUnitHandle_t *evtGetUnitHandle2(dds3Basic_t *unit);

/* Initialize an event unit's color transition for the requested duration. */
void evtSetUnitLightAndAmbient(evtUnitHandle_t *unit, s32 duration, u32 firstColor, u32 secondColor);

void evtSetUnitLightDir(evtUnitHandle_t *unit, EvtUnitDirectionTransitionFrames durationFrames);

void evtUnitLightOn(evtUnitHandle_t *unit);

FldSceneReadyState fldAutoMapChkMode(void);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

void mdlLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

void mdlMotionPause(dds3ModelHandle_t *ctx);

void mdlMotionPauseOff(dds3ModelHandle_t *ctx);

void mdlSetMotionSpeedEx(dds3ModelHandle_t *ctx, MdlMotionSearchId searchId, f32 frameStep);

/* sdfMotionCtrlr_t-record selection and lookup. */
void mdlSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

void mdlSmoothLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames);

void mdlSmoothSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                       f32 blendDurationFrames);

/* Allocate a variable-length sprite column packet with this draw header. */
SdfDrawPacket *sdf2DAllocSprite(s32 loopCount);

SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);

SdfDrawPacket *sdf2DMakePrimPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags,
                                       s32 registerCount, s64 registerList,
                                       s32 loopCount);

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));

/* Append standard packets to a DMA packet list. A null allocator selects the
   default packet allocator. Asset packets reference the selected draw entry. */
void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                              s32 (*allocatePacket)(s32));

/* Build the serialized VIF attribute stream and return its word cursor. */
u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request);

/* Allocate from the chip heap through 0x400 bytes; larger requests use the
 * general heap and return its locked payload address. */
void *sdfAllocAuto(s32 size);

f32 sdfCos(f32 angle);

void *sdfDevPrintfEx(SifCommand *packet, const char *format, ...);

void sdfDevSetPrintCB(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfDrawTagLinkRef(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfDrawTagLinkRefGif(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfFileClose(sdfFileCmdHandle_t *state);

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);

void sdfFileRead(sdfFileCmdHandle_t *state, void *buffer, s32 byteCount);

/* Queue release of the sdfAllocAuto base in its source heap; interior pointers
 * are invalid. */
s32 sdfFreeAutoDelayed(void *data);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void sdfLinkLoadImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

void sdfLinkStoreImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 sourceX, s32 sourceY,
    s32 width, s32 height, s32 resourceAddress, s32 arg7, s32 arg8,
    s32 (*allocatePacket)(s32));

void sdfMemUnlock(struct sdfMemHandle_t *allocation);

/* On success, write the requested MPOS record's position or matrix to VU state. */
SdfMapPositionLookupResult sdfModelGetMarkPos(
    struct dds_sdfModelRootNode_t *model, s32 id);

struct sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));

void sdfPrimLinkGt4(
    sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
    s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
    s32 u2, s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3,
    s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet);

f32 sdfSin(f32 angle);

void sdfTexFreeDelayed(sdfTexHandle_t *texture);

/* Requires a non-NULL texture; returns its texture-owned primary buffer as a
 * borrowed, read-only view. Keep the texture alive while using the packet. */
const struct sdfTexPacket_t *sdfTexGetPacket(const struct sdfTexHandle_t *texture);

/* Requires a non-NULL texture; returns 0 for no primary buffer, otherwise
 * ((gifTagWord & 0x7fff) + 1) * 16 bytes. */
s32 sdfTexGetPacketLength(const struct sdfTexHandle_t *texture);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */
struct sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header);

#include "dds/sdk_ps2.h"

#include "dds/fld_item_box_object_api.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/sdf_gs_header.h"

#include "dds/sdf_pac_reloc.h"

#include "dds/fld_script_resource.h"

#include "dds/sdf_packet_header.h"

#include "dds/eff.h"

#include "dds/fld_player_scene_state.h"

#include "dds/fld_marker_packet.h"

#include "dds/sdf_gs_blend.h"

#include "dds/common.h"

#include "dds/sdf_map_position_api.h"

#include "dds/pty_status_query_api.h"

#include "dds/file_config_option_api.h"

#include "dds/fld_camera_model_motion_request.h"

#include "dds/fld_elv_camera_scene_api.h"

#include "dds/fld_placement_batch_mode.h"

#include "dds/dds3_object_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/fld_stop_entry.h"

#include "dds/sdf_resource_list_release_api.h"

#include "dds/dds3_background_fade_api.h"

#include "dds/fld_movement_hold_state_api.h"

#include "dds/fld_movement_gate_result.h"

#include "dds/fld_scene_ready_api.h"

#include "dds/fld_area_resource_ready_api.h"

#include "dds/fld_area_resource_name_change_api.h"

#include "dds/fld_area_resource_request_reuse_api.h"

#include "dds/fld_file_io_priority_state.h"

#include "dds/fld_location_coordinate_flags.h"

#include "dds/fld_scene_record_api.h"

#include "dds/fld_scene_object_key_result.h"

#include "dds/fld_light_set.h"

#include "dds/fld_sky_buffer.h"

#include "dds/fld_sky_gradient_record.h"

#include "dds/fld_quad_state.h"

#include "dds/dds3_frame_draw_buffer.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_fade.h"

#include "dds/mnu_title_stream.h"

#include "dds/fld_bill_config.h"

#include "dds/sdf_trig.h"

#include "dds/fld_area_path.h"

#include "dds/fld_npc_motion.h"

#include "dds/fld_npc_palette.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/mdl_motion_api.h"

#include "dds/sdf_motion.h"

#include "dds/fld_resource_resolver.h"

#include "dds/fld_clear_menu_entries_api.h"

#include "dds/fld_play_pending_sounds_api.h"

#include "dds/fld_release_texture_slots_api.h"

#include "dds/fld_transfer_chunk.h"

#include "dds/fld_camera_follow_row.h"

#include "dds/fld_named_camera_table.h"

#include "dds/fld.h"

#include "dds/field.h"

#include "dds/fld_encounter_runtime_api.h"

#include "dds/fld_collision_geometry.h"

#include "dds/fld_collision_query_api.h"

#include "dds/fld_scene_request.h"

#include "dds/fld_room_resource_api.h"

#include "dds/fld_area_room_resource_load_api.h"

#include "dds/fld_file_addr_reset_mode.h"

#include "dds/fld_spawn_event.h"

#include "dds/sdf_heap_api.h"

#include "dds/dds3obj.h"

#include "dds/dds3_object_control_slot.h"

#include "dds/evt_world_object_api.h"

#include "dds/dds3_world_value.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_texture_primary_buffer_api.h"

#include "dds/evt_action_object.h"

#include "dds/sdf_texture_offset_list.h"

#include "dds/evt_script_model.h"

#include "dds/sdf_model.h"

#include "dds/mdl_flag_api.h"

#include "dds/dds3_world_counter.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/dds3_world_secondary.h"

#include "dds/eff_light.h"

#include "dds/fld_area_work.h"

#include "dds/fld_spawn_collision.h"

#include "dds/evt_world.h"

#include "dds/evt_world_source_transform.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_column_packet_api.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_packet_builders.h"

#include "dds/fr_font.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_dev_state.h"

#include "dds/fld_waypoint.h"

#include "dds/fld_inf.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/file_request_api.h"

#include "dds/file_pac.h"

#include "dds/fld_packed_resource_kind.h"

#include "dds/fld_title_banner_menu.h"

#include "dds/field_stage.h"

#include "dds/sdf_pac_work.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/fpu.h"

#include "dds/pcp_vu0.h"

#include "dds/sdf.h"

#include "dds/sdf_sif_command.h"

#include "dds/mdl.h"

#include "dds/scr.h"

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_runner.h"

#include "dds/dat_state.h"

#include "dds/dds3_world_primary.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/fld_object_slot_api.h"

#include "dds/fld_placement_data.h"

#include "dds/fld_resource_name.h"

#include "dds/fld_file_resource_link_view.h"

#include "dds/eff_object.h"

#include "dds/dds3_path.h"

#include "dds/fld_label_cache_api.h"

#include "dds/fld_sprite_vertex.h"

#include "dds/fld_room_state.h"

#include "dds/fld_trigger_shape.h"

#include "dds/fld_action_row.h"

#include "dds/btl.h"

#include "dds/itf.h"

#include "dds/fld_index_pair.h"

typedef struct SdfGsTextureAlphaFlushPacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite textureAlpha;
    SdfGsRegisterWrite textureFlush;
} SdfGsTextureAlphaFlushPacket;

typedef char SdfGsTextureAlphaFlushPacket_size_must_be_0x40[
    (sizeof(SdfGsTextureAlphaFlushPacket) == 0x40) ? 1 : -1];

/* Declared before first use (were implicit int). */
extern void btlResetAsyncState(void);

extern void fldDrawCameraColorScrollingGradient(void);

extern FldFileResource *D_003BD7B8;

extern u32 D_003BD7BC;

struct dds3Basic_t;

extern char D_0039FD50[];

struct EffNodeDescriptor;

typedef char FldSpawnBatchSize[(sizeof(FldSpawnBatch) == 12) ? 1 : -1];

extern FldFileResource *D_003BD7B0;

extern u32 D_003BD7B4;

extern u32 D_003C91D0[];

extern struct sdfMemHandle_t *D_003C91E0[];

extern struct EffNodeDescriptor *D_003C91F0[];

extern char D_003BAC40[];

extern dds3Basic_t *effObjCreateResourceKindZero(const char *, void *, s32);

extern dds3Basic_t *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *, void *, void *);

extern FldInfTable D_00332E30;

/* Camera-setting table capacity. */
enum {
    FIELD_CAMERA_SETTING_COUNT = 8
};

typedef struct SdfDrawPacket SdfDrawPacket;

typedef struct DmaPacketHeader DmaPacketHeader;

extern void fldPlayFieldSeVolumePan(s32);

extern void dds3FadeColorSet(s32 red, s32 green, s32 blue);

extern void fldReleaseActorTasksById(s32);

/* Per-actor slot in the field's actor table; reset by fldResetActorSlots and
   scanned by fldReleaseActorTasksById. 0x5C is the stride retail uses. Three transition keys, signed frame
   counters and two float triples with per-frame increments (same layout as DDS2). */
typedef struct {
    s32 kind;
    u32 actorId;
    u32 unk08;
    u32 firstKey;
    u32 secondaryMotion;
    u32 secondKey;
    u32 transitionKey;
    s32 firstFrame;
    s32 secondFrame;
    s32 transitionFrame;
    s32 frameCount;
    f32 firstValues[3];
    f32 firstStep[3];
    f32 secondValues[3];
    f32 secondStep[3];
} FldActorRow; /* 0x5C bytes */

typedef char FldActorRow_size_must_be_0x5C[sizeof(FldActorRow) == 0x5C ? 1 : -1];

extern FldActorRow fldActorSlots[];

extern s32 D_0032E530[];

extern FldNpcMotion D_00336A60[];

extern char *D_003BAE44;

extern char *D_003BAE48;

static inline s32 fldTestBits(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

/* vu0 routine: normalize vf10 and return its original XYZ length. */
static inline f32 fldNormalizeProbeVector(void) {
    f32 length;

    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    return length;
}

extern void fldSpawnActionObjects(const FldActionSpawn *, u32);

extern s32 D_0032E400[];

extern void fldClearCameraObjectHighlightFlag(void);

extern void fldSetCameraObjectHighlightFlag(void);

extern s32 fldApplyPlayerVerticalStep(void);

extern void fldUpdateModeOneCameraPose(void);

extern void fldDispatchCameraModelFrameEvents(s32 motion, s32 selector);

extern s32 fldUpdateFieldPlayerMovement(void);

extern void fldUpdateCameraModelMotion(s32 currentMotionIndex, s32 requestedMotionIndex, f32 blendFrames);

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *, f32);

extern void fldToggleWorldNodeState(s64);

extern void fldProbeCameraSupportSurface(void);

extern void fldRestoreCameraModelColor(void);

extern void fldUpdateCameraFollowPlayer(void);

extern void fldCamUpdateFollow(void);

extern void fldUpdateCameraFollowLinkedObject(void);

extern void fldCamElvTask(void);

extern u8 fldEncountTestFlag(u32);

extern f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz);

extern void fldClearCameraModelColor(void);

extern void fldSetCameraObjectTransitionFlag(void);

extern void fldClearCameraObjectTransitionFlags(void);

extern FldCamRow fldCameraFollowRows[];

extern f32 D_00330630[];

extern s32 D_003BAD24;

extern s32 D_003BAD28;

extern f32 D_00330650[];

extern f32 D_00330660[];

extern void btlActivateRuntime(s32 mode);

extern void *sdf2DMakeStdPacket(void *, s32);

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

extern s32 D_0032E3C0[];

extern s8 D_0032C9A0[];

extern void fldReleaseSkyResources(void);

extern void fldCreatePlayerObject(void);

extern const char *fldSelectCurrentActorOnNextFloor(void);

extern void fldSpawnPlacementObjectsFromBatches(FldSpawnBatch *spawnBatches, u32 batchCount,
                                                FldPlacementBatchMode mode);

extern s32 D_003BAE68;

extern FldWallMeshRecord *fldItemBoxObjBuf;

extern collision_s *fldMatterTable;

extern struct sdfMemHandle_t *fldAuxRecordResource;

extern struct sdfMemHandle_t *fldValueRecordResource;

extern u32 gFilterAlpha;

extern u32 gFilterAnime;

extern f32 gFilterAnimeCntF;

extern s32 gFilterAnimeOfsA;

extern SdfFlagListWork *fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern sdfTexHandle_t *fldRainTextureReference;

extern struct sdfMemHandle_t *fldRainTextureResource;

extern dds3ModelHandle_t *fldPlayerModelContext;

extern dds3Basic_t *fldPlayerObject;

extern void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *spawnedObject);

extern u8 D_003BAB3C;

extern FldCameraModelMotionRequest D_003BAD34;

extern u32 D_003BAD38;

extern u32 D_003BAD3C;

extern u32 D_003BAD40;

extern FldElvCameraSceneMode D_003BAD1C;

extern u32 D_003BACF8;

extern s32 fldDrwZBuf;

extern u32 fldPendingArea;

extern u32 fldPendingFloor;

extern u32 D_003BAD98;

extern u32 D_003BAD9C;

extern f32 D_003BADA0;

extern u32 D_003BADA4;

extern u32 D_003BADC8;

extern u32 D_003BADD8;

extern u32 D_003BAE28;

extern s32 gFldInf_HitIdx;

extern u32 fldSelectedActorEntryIndex;

extern u32 D_0032E428[];

extern u32 D_0032E538[];

extern u32 D_0032E544[];

extern u32 fldRunDanteGunFlag[];

extern s32 D_003BAE38;

extern u32 D_0032E59C[];

extern u32 D_0032E5A8[];

extern u8 sdfViewMatrix[];

extern u32 D_003BACD4;

extern u32 D_003BACEC;

extern sdfTexHandle_t *fldMarkerTexture;

extern u32 D_003BD7C0;

extern u8 D_0032F260[];

extern sdfMat_t *sdfMatNew(void);

extern void fldClrCollision(void);

extern u8 sdfProjectionMatrix[];

extern u8 D_r1_003AD4E0[];

extern char D_003C9200[];

extern s16 D_003C9518[];

extern void fldFormatAreaResourceName(char *out);

extern void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode);

extern f32 D_003BAD20;

extern char fldEncounterTaskName[];

extern f32 D_003C9230[];

extern f32 D_003C9220[];

extern s32 fldEncProc(dds3ProcessID_t *task);

extern void fldResetEncounterAsyncState(dds3ProcessID_t *task);

extern s32 btlUpdateRuntimeFadeState(void);

extern void btlClearRuntimeState(void);

extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *camera, f32 *worldEyeOut,
                                                      f32 *targetPositionOut);

extern f32 sdfAtan2(f32 y, f32 x);

extern u8 D_003BACE0[];

extern u8 D_003BACE8[];

extern u32 D_003BACFC;

extern u32 D_00330738[];

extern u32 D_003306B0[];

extern f32 D_003306C0[];

extern f32 D_00330670[];

extern s32 fldUpdateTitleBannerTask(dds3ProcessID_t *task);

extern s8 btlGetRuntimeResult(void);

extern s32 fldValueRecordCount;

extern s32 D_003BADF8;

extern s32 D_003BADEC;

extern FldFileIoPriorityState D_003BA734;

extern u8 D_003BAC90[];

extern s32 D_0032E4C8[];

extern s32 D_003BAE30;

extern s32 gFldEveRngIdx_N;

extern s32 fldEveRngCount;

extern s16 D_003C9510[];

extern u32 D_003BAE4C;

extern s32 D_003BAE50;

extern void fldDestroyTitleTask(void);

extern void fldReleaseObjectSlots(void);

extern void fldReleaseTitleSlots(void);

extern void fldReleaseResourceHandles();

extern u8 fldTitleMiniIsActive(void);

extern void fldResetZoneRecordsAndActorSlots(void);

extern void fldReleaseMenuSlotsAfterWait(void);

extern void mnuReleaseResourceEntries(void);

extern dds_fldWaypointBlock_t fldActorWaypointRows;

extern FldFileResource *fldRoomResources[];

extern void fldDrawPos(const f32 position[3]);

extern FldIndexPair D_0032EF18[];

extern FldIndexPair D_0032EFE0[];

extern s32 D_003BAE54;

extern s32 D_003BAE5C;

extern s32 D_003BAE60;

extern u32 D_003C91D0[];

extern struct EffNodeDescriptor *D_003C91F0[];

extern struct sdfMemHandle_t *D_003C91E0[];

extern char D_003BAC40[];

extern FldFileResource *D_003BD7B8;

extern u32 D_003BD7BC;

extern s32 fldRoomEventPlacementCount;

extern s16 D_0032E4B4[];

#include "dds/fld_event_display_resource_api.h"

extern void func_00148FF0(s32, u32, f32 *, f32, f32, f32);

extern void func_001480D0(u32, f32 *, f32, f32, f32, s32, const char *, s32);

extern void func_001486D0(u32, f32 *, f32, f32, f32);

extern s32 fldSetSparkVectors(s32 index, const u128 *position, const u128 *rotation);

/* Serialized transforms leave the position W component zero. */
static inline void fldCopySpawnTransform(f32 *position, f32 *rotation, const f32 *source) {
    if (source != NULL) {
        position[0] = source[0];
        position[1] = source[1];
        position[2] = source[2];
        position[3] = 0.0f;
        rotation[0] = source[4];
        rotation[1] = source[5];
        rotation[2] = source[6];
        rotation[3] = source[7];
    } else {
        position[0] = 0.0f;
        position[1] = 0.0f;
        position[2] = 0.0f;
        position[3] = 0.0f;
        rotation[0] = 0.0f;
        rotation[1] = 0.0f;
        rotation[2] = 0.0f;
        rotation[3] = 0.0f;
    }
}

void fldSpawnPlacementObjectsFromBatches(FldSpawnBatch *spawnBatch, u32 batchCount, FldPlacementBatchMode mode);

void fldSpawnActionObjects(const FldActionSpawn *spawnList, u32 spawnCount);

extern FldFileResource *D_003BD7B0;

extern u32 D_003BD7B4;

extern FldFileResource *D_003BD7B8;

extern u32 D_003BD7BC;

extern struct DevRequest *fldRoomSoundResources;

void fldCreateResourceScriptObjects(void);

extern u8 fldTestMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit);

extern u8 fldTestRoomModeFlag(s32 mapId, u32 slotIndex, s32 bit);

extern char D_0039FE08[], D_0039FE18[], D_0039FE28[];

void fldLoadSceneRequestFiles(const FldLoadRequest *request);

/* Handle a field request, creating the player only in non-special scene states. */
void fldProcessFieldRequest(const FldSceneRequest *request);

void fldProcessFieldRequestAlternate(const FldSceneRequest *request);

void fldRelocateTransferChunkWords(u8 *buffer, const FldTransferChunk *chunk);

extern char D_0039FE38[];

FldBattleReturnAreaLoadResult fldTryStartPendingBattleReturnAreaLoad(void);

extern void sdfFileCmdThreadSetLoPrio(void);

extern s32 D_003BAC80;

s32 fldRequestAreaResource(s32 area, s32 room);

u32 fldPollAreaResourceLoad(void);

FldNextFileLoadState fldGetNextFileLoadState(void);

FldAreaResourceReadyState fldIsAreaResourceReady(void);

FldAreaResourceRequestReuseResult fldCanReuseAreaResourceRequest(s32 area, s32 room);

extern const u8 *fldCachedRoomResourceData, *fldCachedRoomF1ResourceData, *fldCachedRoomF2ResourceData,
    *fldCachedRoomKF2ResourceData;

extern u32 fldCachedRoomResourceSize, fldCachedRoomF1ResourceSize, fldCachedRoomF2ResourceSize, fldCachedRoomKF2ResourceSize;

struct sdfMemHandle_t *fldCopyCachedRoomResourceIfLocationMatches(const SdfTextureOffsetListHeader **destination, s32 area, s32 room);

struct sdfMemHandle_t *fldCopyCachedRoomF1ResourceIfLocationMatches(u8 **destination, s32 area, s32 room);

struct sdfMemHandle_t *fldCopyCachedRoomF2ResourceIfLocationMatches(u8 **destination, s32 area, s32 room);

struct sdfMemHandle_t *fldCopyCachedRoomKF2ResourceIfLocationMatches(u8 **destination, s32 area, s32 room);

void fldFormatAreaResourceName(char *out);

FldAreaResourceNameChangeResult fldHasAreaResourceNameChanged(void);

extern void fldAdoptSceneRecordChunk(u8 *chunk, struct sdfMemHandle_t *resourceHandle);

extern s32 fldRoomEventPlacementCount;

extern FldTransferChunk *fldRoomF1Chunk;

extern u8 *fldRoomF1Data;

extern FldTransferChunk *fldRoomF2Chunk;

extern u8 *fldRoomF2Data;

extern FldTransferChunk *fldRoomKF2Chunk;

extern u8 *fldRoomKF2Data;

extern FilePacRequest *D_003BAC38;

extern struct sdfMemHandle_t *D_003BAC4C;

extern struct sdfMemHandle_t *fldCachedRoomResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF1ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF2ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomKF2ResourceAllocation;

extern void fldSetSceneLocation(s32 area, s32 floor, s32 stage);

extern void fldLoadPlayerModel(void);

extern void fldInitSparkTable(void);

extern void fldInitializeMenuResources(void);

extern void fldUpdateSparkSlots(void);

extern void fldInitializeActorMotionSlots(void);

extern char D_003BAC98[];

extern char D_003BACA0[];

extern char D_003BACA8[];

extern void fldCreateResourceScriptObjects(void);

extern const char D_0039FF18[16];

const struct sdfMemHandle_t *fldLoadAreaRoomResources(const char *resourceName);

extern s32 fldRoomEventPlacementCount;

extern FldTransferChunk *fldRoomF1Chunk;

extern u8 *fldRoomF1Data;

extern FldTransferChunk *fldRoomF2Chunk;

extern u8 *fldRoomF2Data;

extern FldTransferChunk *fldRoomKF2Chunk;

extern u8 *fldRoomKF2Data;

extern FilePacRequest *D_003BAC38;

extern struct sdfMemHandle_t *D_003BAC4C;

extern struct sdfMemHandle_t *fldCachedRoomResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF1ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF2ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomKF2ResourceAllocation;

void fldReleaseFieldResources(void);

extern u8 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit);

extern char D_003BACB0[];

extern char D_003BACB8[];

extern char D_003BACC0[];

extern char D_0039FF88[];

extern u8 fldTestRoomObjectModeFlag(s32 mapId, u32 slotIndex, s32 bit);

extern char D_003BACB0[];

extern char D_003BACB8[];

extern char D_003BACC0[];

void fldDrawInit(void);

sdfGrOt_t *fldDrawGetOt(void);

void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture);

void DrwSprite2DC(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture);

void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture);

extern u8 D_00324650[];

void fldPoint3To2(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z);

extern u8 D_003249B0[];

extern u8 D_003249F0[];

extern u8 D_00324A00[];

extern u8 D_00329790[];

void fldPoint3To2H(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z);

/* vu0 routine: prepare the shared projection and secondary matrix bank. */
void fldPrepareProjectionMatrix(void);

void fldProjectPointWithPreparedMatrix(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z);

void rsetOtOfs(u32 displayRow);

void rsetTexa(s32 lower, s32 bits, u64 upper);

void rsetTest(s32 bit0, s32 bit1, s32 bit4, s32 bit12, s32 bit14, s32 bit15, s32 unused, s32 bit17);

/* Write the selected blend equation to GS ALPHA_1. */
void rsetAlpha(s32 mode);

void aDrawLineStripPrim(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2);

void aDrawLineDirect(u32 x0, u32 y0, u32 x1, u32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2);

void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3);

void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 vertexTag, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3);

void aDrawBoxF_Direct(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3);

void aDrawTriG(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2);

void fldSubmitGsGradientQuad(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);

void aDrawBoxGF(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h);

void fldSubmitTaggedGsRectangle(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag);

extern void sdf3DMakeDirectRefPacket(DmaPacketHeader *, u32, s32);

extern void sdfBillBoardMakeTMPacket(u8 *, const f32 *);

extern void sdfBillBoardMake1(u8 *, const f32 *, u32, const BillTextureQuad *, const f32 *);

void drawSprite3D(sdfTexHandle_t *texture, FldMarkerPacket *modelData);

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

void fldSubmitPrimaryFramePacket(void);

void fldSubmitAlternateFramePacket(void);

extern f32 D_0032F4E0[][4];

extern u32 D_0032F500[];

void ooyTestLine3D(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w);

void fldDrawPoly3(s32 color0, s32 color1, s32 color2,
                         f32 x0, f32 y0, f32 z0,
                         f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2);

/* Submit the field overlay's fixed register state and textured sprite payload. */
void fldSubmitTexturedOverlaySprite(void);

/* Submit the field overlay's fixed register state and textured sprite payload. */
void fldSubmitOverlaySpriteWithRenderState(s32 mode);

/* Submit the field overlay's fixed register state and textured sprite payload. */
void fldSubmitOverlayStateAndSprite(s32 alpha);

void fldDrawPos(const f32 position[3]);

void fldDrawMarkerQuadColored(const f32 position[3], s32 color);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

void fldDraw2DBar(s32 x, s32 y, s32 width, s32 height);

extern sdfGrFrameSet_t sdfGrFrameSet;

void fldDrwZSave(void);

void fldDrwZLoad(void);

void fldKanjiDraw();

void LF(FldQuadState *quad);

void initDrawTag(FldQuadState *quad);

extern sdfGrOt_t D_00325708;

extern sdfGrOt_t sdfDebugGrOt;

extern void InversMatrix(void);

void fldDbgPrint3D(f32 x, f32 y, f32 z, s32 drawValue);

void fldDbgPrint2DC(s32 x, s32 y, s32 packetField, s32 drawValue);

void fldDbgPrintNum2D(s32 x, s32 y, s32 drawValue);

void fldDbgPrintNum2DH(s32 x, s32 y, s32 drawValue);

void fldDbgPrintStr2D(s32 x, s32 y, s32 drawValue);

void fldDbgPrintStr2DC(s32 x, s32 y, s32 packetField, s32 drawValue);

void fldDbgPrintStr2DCF(s32 packetField, s32 drawValue, f32 x, f32 y);

void fldDrawMaruKage(u32 fade, f32 x, f32 y, f32 z, f32 radius);

void fldSubmitAreaOverlayQuads(s32 alpha, s32 offset);

FldEncounterRuntimeResult fldGetEncounterRuntimeResult(void);

void fldSetEncounterPendingValue(u32 pendingValue);

dds3TaskUpdateResult_t fldEncProc(dds3ProcessID_t *task);

extern void p2sdPlaySE(s32 sequence, s32 volume, s32 pan);

void fldRequestEncounterWithFade(FldEncounterRuntimeMode encounterMode,
                                 FldBattleSceneRecordIndex recordIndex);

s32 fldSetEncounterMode(FldEncounterRuntimeMode encounterMode);

void fldResetEncounterAsyncState(dds3ProcessID_t *task);

void fldCreateEncounterTask(void);

extern f32 fldLookAtNearPoint[];

extern f32 fldCamTarget[];

extern f32 D_0032E41C[];

void fldUpdateLookAtSegmentDistance(void);

extern f32 fldCamCalcSlopeOffset(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs);

/* Probe the support surface under the camera's focus and its mirrored heading. */
void fldProbeCameraSupportSurface(void);

extern f32 EulerAngleAxisY(void);

extern f32 D_003BAD10, D_003BAD14, D_003BAD18;

typedef union FldLookAtVector {
    u128 q;
    f32 f[4];
} FldLookAtVector;

void fldUpdateFieldCameraViewState(void);

/* Follow the player with the field camera and pull the far point back to the collision hit. */
void func_0012CED0(void);

void fldUpdateCameraProjectionEndpoints(void);

void fldCamUpdateFollow(void);

extern s32 D_003BAB50;

extern s32 D_003BAD30;

extern void dds3CameraObjectBasicSetWorldEye(dds3Basic_t *camera, const u128 *worldEye);

void fldUpdateCameraFollowPlayer(void);

extern void dds3CameraObjectBasicSetWorldEye(dds3Basic_t *camera, const u128 *worldEye);

extern f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

extern f32 EulerAngleAxisY(void);

extern void dds3GetTMPoseSet(dds3Basic_t *object);

void fldUpdateCameraViewFromPlayer(void);

/* Suppress the world object's current entry when it is already selected. */
dds3Basic_t *fldCamNowEveCamera(void);

void fldSetElvCameraScene(FldElvCameraSceneMode mode);

extern void dds3OnPoseSetFlag(dds3PoseSet_t *, u32);

void fldCamElvTask(void);

void fldClearCameraMoveMode(void);

typedef struct FldModelMatrices {
    u8 pad00[0xC];
    u8 **rows; /* 0x0C: slot-indexed matrix blocks */
} FldModelMatrices;

void fldUpdateCameraProximity(void);

s32 fldUpdateCameraFollow(void);

void fldResetCameraModelHandles(void);

void fldReleaseCameraModel(u32 enabled);

void fldSetCameraModelMotionRequest(FldCameraModelMotionRequest request,
                                    u32 frameCountdown);

void fldUpdateCameraModelMotion(s32 currentMotionIndex, s32 requestedMotionIndex, f32 blendFrames);

struct EffRandState;

extern u32 dds3GetRandA(struct EffRandState *state);

extern void fldSetSequenceVolume(s32 category, s32 volume);

extern void fldQueuePrimaryEffectPosition(f32 x, f32 y, f32 z);

extern void fldQueueSecondaryEffectPosition(f32 x, f32 y, f32 z);

extern s32 D_003BAD44;

/* Dispatch camera-model frame crossings to sequence sounds and queued effects. */
void fldDispatchCameraModelFrameEvents(s32 motion, s32 selector);

extern s32 D_003BAB50;

extern void fldHitChkUnitElv(f32 *);

void fldUpdateCameraTarget(void);

extern const PcpVectorCopyF32 D_003A0048;

extern f32 fldAngleDifference(f32 fromAngle, f32 toAngle);

extern void FromRotation(f32 angle);

extern void fldRefreshAreaStateFromPosition(f32 *);

/* Follow the selected mode-1 heading while preserving field position history.
 * The four legacy SDK stores are qualified only for this pinned emitted body;
 * each vector consumer and conditional join requires a fresh audit on changes. */
void fldUpdateModeOneCameraPose(void);

/* Signed verticalStepDirection: adjust player Y by -2/+2 and save previous XYZ. */
/* Publish the vertical step to field state, then rebuild the object vector.
 * Each branch-local SDK snapshot dominates all of its scalar reads; there is
 * no earlier scratch read or loop-carried value. The finite qualification
 * and memory-contract limitation are documented in docs/idioms.md. */
s32 fldApplyPlayerVerticalStep(void);

extern void mnuTitleStreamUpdateAndLogBgm(void);

extern void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

extern void fldEncountSetFlag(u32 mask);

/* Advance the field transition while the camera model's motion drives completion. */
FldMovementGateResult fldUpdateFieldTransition(void);

FldMovementGateResult fldUpdateAreaDamageReaction(void);

extern s8 dds3PadButtonState[2][2][16];

extern u8 D_003BA870[2][2][2];

extern s32 D_003BAD2C;

extern s32 D_003BAD30;

extern s32 D_003BAD48;

extern s32 D_003BAD4C;

extern f32 D_003BAD50;

extern s32 D_003BAD54;

extern f32 D_0032F170[12];

extern f32 fldSnapAngleToCompassPoint(f32);

extern f32 fldPlayerCalc360_8(f32);

extern f32 fldPlayerCalc360_4(f32);

extern u8 fldIsNearSpark(f32 x, f32 y, f32 z);

extern void fldHitChkUnitDamage(f32 *);

extern void fldRestoreCameraModelColor(void);

extern void fldClearCameraModelColor(void);

extern void fldDisableCameraObjectFlag(void);

extern void fldSetCameraObjectTransitionFlag(void);

extern void fldSetCameraObjectHighlightFlag(void);

s32 fldUpdateFieldPlayerMovement(void);

/* Field camera model: node id at +0x12, render-data pointer at +0x18. */
s32 fldUpdateCameraFrame(void);

FldMovementHoldState fldGetMovementHoldState(void);

void fldSetCameraNodeModeWithZero(void);

void fldEnableCameraObjectFlag(void);

void fldDisableCameraObjectFlag(void);

void fldClearCameraObjectHighlightFlag(void);

void fldSetCameraObjectHighlightFlag(void);

void fldClearCameraModelColor(void);

void fldRestoreCameraModelColor(void);

void fldSetCameraObjectTransitionFlag(void);

void fldClearCameraObjectTransitionFlags(void);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *, u32);

/* State-first XYZ publication; SDK snapshot audit: docs/idioms.md. */
s32 fldUpdateCameraObjectPose(void);

extern void fldUpdateCameraModelMotion(s32 currentMotionIndex, s32 requestedMotionIndex, f32 blendFrames);

void fldSetCameraNodeModeWithTen(void);

void fldSetCameraNodeModeWithZero(void);

void fldAddCameraModelEntry(s32 motionIndex);

void fldAddCameraModelPair(s32 firstMotionIndex, s32 secondMotionIndex);

void fldClearCameraFacingPointRequest(void);

void fldClearCameraHeadingRequest(void);

/* Camera facing requests share the field-work block. Both request states
 * advance from 1 to 2 when their new angle is installed. */
void fldQueueCameraXYOverride(f32 targetX, f32 targetZ);

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z);

void fldApplyCameraFacingPoint(void);

void fldApplyPendingCameraHeading(void);

extern const PcpVectorCopyF32 D_003A00A8;

/* Advance a pending heading, retain position history, and export the player's
 * smoothed position and rotation. Vertical movement bypasses position damping. */
void fldUpdateSmoothedCameraPose(void);

extern SdfFlagListParams fldCameraColorParameters[];

extern FldCameraSetting *fldCameraSettings;

extern FldCameraSetting D_003306D0;

extern FldLightSet *D_003BAD60;

extern FldSkyGradientRecord *D_003BAD74;

extern s32 D_003BAD88, D_003BAD8C;

extern f32 D_003BAD90, D_003BAD94;

void fldLoadBattleSkyAndFilter(void);

extern char D_003A0100[];

extern u32 fldRainTextureData;

void fldLoadSkyResource(s32 area);

void fldReleaseSkyResources(void);

void fldUploadSkyBuffer(const FldSkyBuffer *src);

enum {
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_32 = 1,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_32 = 2,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_32 = 3,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_48 = 4,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_48 = 5,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_48 = 6,
};

void fldSkyCalcFilter(void);

/* Draw the current filter grid, blending the previous grid during a fade. */
void fldSkyDrawFilter(void);

extern s32 D_003BAD58;

void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);

void fldSetFilterAnime(u32 mode);

void fldSkySetAlpha(u32 drawState);

u32 fldSkyGetAlpha(void);

void fldSkySetAlphaOfs(u32 alpha);

u32 fldSkyGetAlphaOfs(void);

void fldSkySetTableAnime(u32 transitionMode);

extern char D_003A0138[];

/* Alternate the selected value at the requested rate, and apply live lighting edits. */
void fldUpdateSelectedValueTransition(void);

/* VU0 direction setters consume vf10, as in the DDS2 lighting routine. */
void fldSetPlayerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                   f32 redB, f32 greenB, f32 blueB,
                   f32 x, f32 y, f32 z);

extern void dds3SetFogColor(s32, f32 *);

extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

extern void dds3SetAmbientColor(s32, f32 *);

extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

extern void fldSetFilterAnime(u32);

extern u32 D_003BADA8;

extern void fldSetPlayerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                          f32 redB, f32 greenB, f32 blueB, f32 x, f32 y, f32 z);

/* Start fading to the sky light set at index. A zero scene command also applies the set to the
 * display state and player lighting using the (clamped) transition duration. */
void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode);

extern s32 D_0032E3C0[];

extern s16 D_0032E4D8[];

extern s32 D_003BAD58;

void fldApplyPendingSceneValueWithSpeed(s32 speed);

/* The resolver's selected-light transition cache is stored in these existing
 * D1 sdata words; the light record itself is the complete 0xE0-byte owner. */
extern u32 D_003BADA8;

extern u32 D_003BADAC;

extern f32 D_003BADB0;

extern s32 D_003BADB4;

extern s32 D_003BADB8, D_003BADBC;

extern f32 D_003BADC0, D_003BADC4;

extern u32 D_003BADCC;

extern f32 D_003BADD0;

extern u32 D_003BADD4;

extern void dds3SetFogColor(s32, f32 *);

extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

extern void dds3SetAmbientColor(s32, f32 *);

extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

void fldTransitionSkyLightSet(s32 lightSetIndex, s32 mode);

void fldSkySetTblLightMaDZ(s32 enabled);

/*BEGIN fldApplySceneLightSet*/
extern void dds3SetFogColor(s32, f32 *);

extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

extern void dds3SetAmbientColor(s32, f32 *);

extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

extern void fldSetFilterAnime(u32);

extern u32 D_003BADDC;

extern void fldSetPlayerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                          f32 redB, f32 greenB, f32 blueB, f32 x, f32 y, f32 z);

void fldApplySceneLightSet(s32 lightSetIndex, s32 durationMode);

void fldInitializeDisplayPointerTable(void);

extern void dds3SetFogColor(s32, f32 *);

extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

extern void dds3SetAmbientColor(s32, f32 *);

extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

extern void fldSetFilterAnime(u32);

void fldApplySkyLightSetToPlayerVU(void);

void fldSkyBattle(void);

void fldApplyLightSetIndex(s32 index);

extern FldCameraSetting fldAppliedCameraSettings[];

void fldActivateCameraColorSetting(s32 enable);

extern s32 fldCameraColorFade;

s32 fldComposeFadeColor(s32 fade, s32 color, s32 alpha);

extern char D_003A0100[];

extern u32 fldRainTextureData;

void fldInitializeCameraColorResource(void);

void fldDrawCameraColorLayerPass(void);

void fldReleaseCameraColorEffect(void);

void fldCopyCameraSetting(FldCameraSetting *destination);

void fldUpdateCameraColorEffect(const FldCameraSetting *setting);

/* Keep both the resource handles and retained addresses: callers use the
 * retained storage, whereas the handles are needed at release time. */
void fldAllocPlaneTable(void);

/* Release both retained resources and clear the usable buffer addresses. */
void fldFreeMatterTable(void);

/* Dot product of XYZ only; any fourth vector component is ignored. */
float fldVecDot(const float *left, const float *right);

/* Return the Euclidean XYZ length without normalizing the input vector. */
f32 MATH_VectorLength(const f32 *vector);

/* Build a normalized plane from three vertices spaced four floats apart.
 * Snap near-zero normal components before deriving D from the third vertex.
 * Degenerate triangles retain the native divide-by-zero behavior. */
void fldMakeEveHitPlanes(const f32 *vertices, f32 *normalX, f32 *normalY, f32 *normalZ, f32 *planeConstant);

/* Return the first matching record's world node; NULL also denotes a missing ID. */
dds3Basic_t *fldFindWorldNodeForRecordId(s32 recordId);

/* Update every matching ID, including duplicates; leave other records intact. */
void fldBindWorldNodeToRecordId(s32 recordId, dds3Basic_t *worldNode);

void fldClrCollision(void);

s32 HitPositionInside2(f32 *out, FldCollisionAxis axis, s32 count, f32 margin, const f32 *pos,
                        const collision_s *zone);

extern f32 fldResolvedPosition[3];

extern u8 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit);

extern u8 fldTestMapSlotAuxiliaryFlag(
    s32 mapId, u32 slotIndex, FldMapSlotAuxiliaryFlagSelector bit);

void fldHitChkUnitElvOnly(const f32 *input);

/* Indexed fields retain index-first addresses; see docs/idioms.md,
 * "Trace-justified indexed field addresses: relative zone motion". */
void fldHitChkUnitElv(f32 *position);

/* Non-owning native alias of fldGlobalBase.terrainAttritionKind (FldAreaWork + 0x11C). */
extern FldTerrainAttritionKind D_0032E4CC[];

/* Classify flagged zone faces after checking the current movement. */
void fldHitChkUnitDamage(f32 *position);

void func_00137E90(void);

void func_00137E98(void);

void func_00137EA0(void);

/* Project away the selected axis and reject points outside expanded bounds.
 * Return the last negative plane's margin-adjusted distance, not a minimum;
 * no negative plane leaves margin unchanged. Reject results below 0.001. */
f32 HitPositionInside3(FldCollisionAxis axis, s32 planeCount, f32 margin, const f32 *position,
                       const collision_s *zone);

FldValueRecordIndex fldHitChkCamera(const f32 start[3], const f32 end[3], f32 hitPoint[3]);

FldValueRecordIndex fldHitChkEnemy(const f32 start[3], const f32 end[3], f32 hitPoint[3]);

/* Add the four vertical faces of an axis-aligned field collision box. */
void fldAddItemBoxObject(f32 x, f32 y, f32 z, f32 extent, f32 height,
                         FldMapSlotAuxiliaryFlagSelector auxiliaryFlagSelector);

void evtMessageExtraActionNoOp(void);
/* End of declarations from src/dds1/field/fldFileResolver.c */

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldMakeBlockBasicFL2);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132100);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132308);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132390);

void fldProcessFieldRequestAlternate(const FldSceneRequest *request) {
    fldSpawnPlacementObjectsFromBatches(request->spawnBatches, request->spawnBatchCount,
                                        FLD_PLACEMENT_BATCH_MODE_APPEND);
}

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_001323E8);

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

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_001324D8);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132500);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132528);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132530);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132538);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132540);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132548);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132550);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132558);

void func_r1_00132560(void) {
}

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_00132568);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", func_r1_001325D8);
INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B6F0);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B6F4);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B6F8);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B6FC);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B700);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B704);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B708);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B70C);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B710);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B714);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B718);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B71C);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", D_r1_0046B720);

