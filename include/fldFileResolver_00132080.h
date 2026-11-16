#ifndef FLD_FILE_RESOLVER_00132080_H
#define FLD_FILE_RESOLVER_00132080_H

#include "draw.h"
#include "common.h"
#include "field.h"
#include "basic.h"
#include "model.h"
#include "motion.h"

/* sdfGrPacketHeader_t stored as four plain words. The dmatag union in sdfGrPacketHeader_t reorders
 * ee-gcc's stores in effDrawGeneratedTextureQuad; retail stores in this type's order. */
typedef struct sdfGrPacketHeaderStore_t {
    u64 dmaTag;
    u64 vifCommands;
    u64 gifTag;
    u64 gifRegisters;
} QWORD_ALIGNED sdfGrPacketHeaderStore_t;

typedef char sdfGrPacketHeaderStore_t_size_must_be_0x20[
    (sizeof(sdfGrPacketHeaderStore_t) == 0x20) ? 1 : -1];

typedef u32 FldSpawnBatchKind;

/* Counted resource group consumed by field spawning and copied payloads. */
typedef struct fldSpawnBatch_t {
    FldSpawnBatchKind kind;
    u32 count;
    fldResHead_t *entries;
} fldSpawnBatch_t;

/* fldSpawnBatch_t.kind values. */
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

typedef char fldFileNameEntrySizeCheck[(sizeof(fldFileNameEntry_t) == 0x0C) ? 1 : -1];

struct fldConstruntAreaFL2_t;
struct fldTransferChunk_t;
struct fldFileInfoHeaderFL1_t;
struct fldFileInfoHeaderFL2_t;
struct sdfTextureOffsetListHeader_t;

/* The collision batch fldMakeResouceBasicFL2 last spawned, searched by MakeObjectBasic2. */
extern fldResHead_t *g_pHitData;
extern u32 g_nHitData;
extern u32 MakeResouceBasicFL2Data;

void MakeObjectBasic2(void);
void fldMakeResouceBasicFL1(void *typeTable, u32 batchCount);
void fldMakeResouceBasicFL2(fldSpawnBatch_t *batch, u32 batchCount, s32 mode);

/* FL1 resource file names, and the handle and data of each loaded file. */
extern char *fldResolverFL1Data[4];
extern struct sdfMemHandle_t *fldD3pHandle[4];
extern void *fldD3pData[4];
u32 fldLoadFile(void);
void fldMakeBlockBasicFL2(struct fldConstruntAreaFL2_t *pBlockTable, u32 nBlock);
void fldFileResolverFL1(struct fldFileInfoHeaderFL1_t *info);
void fldFileResolverFL2(struct fldFileInfoHeaderFL2_t *info);
u8 *fldGetInfoHeader(struct fldTransferChunk_t *pData);
void fldAddressResolver(addr_t pFileTop, struct fldTransferChunk_t *pData);
void fldAddressResolverReset(addr_t pFileTop, struct fldTransferChunk_t *pData);
void fldLeaf16(void);
u32 fldLoadFile_FromMemory(u32 field, u32 area, saddr_t pF1, saddr_t pF2,
                           const struct sdfTextureOffsetListHeader_t *pTB, s32 pBF);

/* State fldLoadFile_FromMemory resets and fills: the reset mode, file
 * buffers, the texture list, and the FL2 and FL1 base and chunk addresses. */
extern s32 fldData16;
extern u32 fldData17;
extern void *fldCallTexListFreeData8;
extern void *fldCallTexListFreeData9;
extern void *fldCallTexListFreeData10;
struct sdfItemArray_t;
struct sdfTextureOffsetListHeader_t;
extern void *fldData15;
struct sdfItemArray_t *sdfTexSetTB(const struct sdfTextureOffsetListHeader_t *aData);
extern saddr_t fldCallTexListFreeData;
extern saddr_t fldCallTexListFreeData2;
extern saddr_t fldCallTexListFreeData3;
extern saddr_t fldCallTexListFreeData4;
extern saddr_t fldCallTexListFreeData5;
extern saddr_t fldCallTexListFreeData6;
extern u32 fldCallTexListFreeData7;
dds3Basic_t *dds3AddModelBasic2(s32 serialID, sdfModelDef_t *pModelDef, sdfMotionPlayBook_t *pMotDef, void *pTexList, char *pName);

#endif
