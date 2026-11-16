#ifndef FLD_FILE_RESOLVER_00132080_H
#define FLD_FILE_RESOLVER_00132080_H

#include "draw.h"
#include "common.h"
#include "field.h"

/* sdfGrPacketHeader_t's layout as plain words: its dmatag union reorders ee-gcc's stores
 * in effDrawGeneratedTextureQuad, which retail emits in this type's order. */
typedef struct SdfGsPacketHeader {
    u64 dmaTag;
    u64 vifCommands;
    u64 gifTag;
    u64 gifRegisters;
} QWORD_ALIGNED SdfGsPacketHeader;

typedef char SdfGsPacketHeader_size_must_be_0x20[
    (sizeof(SdfGsPacketHeader) == 0x20) ? 1 : -1];

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
