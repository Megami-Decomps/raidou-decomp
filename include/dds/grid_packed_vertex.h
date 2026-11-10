#ifndef DDS_GRID_PACKED_VERTEX_H

#define DDS_GRID_PACKED_VERTEX_H
#include "common.h"

/* One packed grid vertex shared by line, triangle, and strip emitters. */
typedef struct GridPackedVertex {
    u64 colorChannels[2]; /* 0x00 */
    u64 packedXY;          /* 0x10 */
    u64 depth;             /* 0x18 */
} GridPackedVertex;

typedef char GridPackedVertex_size_must_be_0x20[
    (sizeof(GridPackedVertex) == 0x20) ? 1 : -1];

#endif /* DDS_GRID_PACKED_VERTEX_H */
