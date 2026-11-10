#ifndef DDS_SDF_HEAP_API_H

#define DDS_SDF_HEAP_API_H
#include "common.h"

/* General-heap descriptors are distinct from the data addresses they own. */
struct sdfMemHandle_t;

/* Six signed words written by sdfMemGetInfo. */
typedef struct SdfGeneralHeapStats {
    s32 totalBytes;
    s32 freeBytes;
    s32 largestFreeBytes;
    s32 smallestFreeBytes;
    s32 blockCount;
    s32 freeBlockCount;
} SdfGeneralHeapStats;

/* Five totals followed by the seven native chip size-class counters. */
typedef struct SdfChipHeapStats {
    u32 totalBytes;
    u32 freeBytes;
    u32 blockCount;
    u32 emptyBlockCount;
    u32 partialBlockCount;
    u32 usedCells[7];
} SdfChipHeapStats;

typedef char SdfGeneralHeapStats_size_must_be_0x18[
    (sizeof(SdfGeneralHeapStats) == 0x18) ? 1 : -1];

typedef char SdfChipHeapStats_size_must_be_0x30[
    (sizeof(SdfChipHeapStats) == 0x30) ? 1 : -1];

#endif /* DDS_SDF_HEAP_API_H */
