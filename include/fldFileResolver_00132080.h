#ifndef FLD_FILE_RESOLVER_00132080_H
#define FLD_FILE_RESOLVER_00132080_H

#include "draw.h"
#include "common.h"

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

/* fldFileResolver::fldMakeResouceBasicFL1 publishes file rows; fldSpawnPlacementObjectsFromBatches walks them
 * at stride 0x24. Both game resolvers walk counted names at stride 0x0C.
 * Name tables have variable extent; entries[1] is the native struct-hack head. */
typedef struct FldFileNameEntry {
    u32 word00;
    const char *name;
    u32 word08;
} FldFileNameEntry;

typedef struct FldFileNameTable {
    u32 count;
    FldFileNameEntry entries[1];
} FldFileNameTable;

typedef struct FldFileResource {
    u32 id;
    u32 word04;
    const char *name;
    u32 word0C;
    const f32 *transform; /* Serialized translation and rotation input. */
    u32 word14;
    FldFileNameTable *names;
    u32 word1C;
    void *data;
} FldFileResource;

typedef u32 FldSpawnBatchKind;

/* Counted resource group consumed by field spawning and copied payloads. */
typedef struct FldSpawnBatch {
    FldSpawnBatchKind kind;
    u32 count;
    FldFileResource *entries;
} FldSpawnBatch;

typedef char FldFileNameEntrySizeCheck[(sizeof(FldFileNameEntry) == 0x0C) ? 1 : -1];

typedef char FldFileResourceSizeCheck[(sizeof(FldFileResource) == 0x24) ? 1 : -1];

struct fldConstructAreaFL2_t;

void MakeObjectBasic2(void);
void fldLoadFile(void);
void fldMakeBlockBasicFL2(struct fldConstructAreaFL2_t *pBlockTable, u32 nBlock);

#endif
