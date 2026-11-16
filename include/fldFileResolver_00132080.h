#ifndef FLD_FILE_RESOLVER_00132080_H
#define FLD_FILE_RESOLVER_00132080_H

#include "draw.h"
#include "common.h"
#include "field.h"

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

typedef char fldFileNameEntrySizeCheck[(sizeof(fldFileNameEntry_t) == 0x0C) ? 1 : -1];

struct fldConstructAreaFL2_t;

void MakeObjectBasic2(void);
void fldLoadFile(void);
void fldMakeBlockBasicFL2(struct fldConstructAreaFL2_t *pBlockTable, u32 nBlock);

#endif
