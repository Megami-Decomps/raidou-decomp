#ifndef DDS_FLD_SPRITE_VERTEX_H

#define DDS_FLD_SPRITE_VERTEX_H
#include "common.h"

/* One corner in the 0x50-byte payload of a single-column sprite packet.
 * The producer writes texture coordinates, screen coordinates, mask and flag;
 * the two intervening spans remain opaque. */
typedef struct FldSpriteCorner {
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} FldSpriteCorner;

typedef struct FldSpriteVertex {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    FldSpriteCorner corner[2];
} FldSpriteVertex;

typedef char FldSpriteCornerSizeCheck[(sizeof(FldSpriteCorner) == 0x20) ? 1 : -1];

typedef char FldSpriteVertexSizeCheck[(sizeof(FldSpriteVertex) == 0x50) ? 1 : -1];

#endif /* DDS_FLD_SPRITE_VERTEX_H */
