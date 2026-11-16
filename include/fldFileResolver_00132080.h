#ifndef FLD_FILE_RESOLVER_00132080_H
#define FLD_FILE_RESOLVER_00132080_H

#include "draw.h"
#include "common.h"
#include "field.h"

/* Appending the packet patches the DMA tag's link; the VIF and GIF words
 * retain the renderer's original commands and register-list encoding. */
typedef struct SdfGsPacketHeader {
    u64 dmaTag;
    u64 vifCommands;
    u64 gifTag;
    u64 gifRegisters;
} QWORD_ALIGNED SdfGsPacketHeader;

typedef struct SdfGsRegisterWrite {
    u64 value;
    u64 registerId;
} SdfGsRegisterWrite;

typedef char SdfGsPacketHeader_size_must_be_0x20[
    (sizeof(SdfGsPacketHeader) == 0x20) ? 1 : -1];

typedef char SdfGsRegisterWrite_size_must_be_0x10[
    (sizeof(SdfGsRegisterWrite) == 0x10) ? 1 : -1];

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
