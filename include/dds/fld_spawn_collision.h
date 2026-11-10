#ifndef DDS_FLD_SPAWN_COLLISION_H

#define DDS_FLD_SPAWN_COLLISION_H
#include "common.h"

struct FldCollisionDescriptor;

typedef struct FldSpawnCollision {
    u32 kind;             /* 0x00 */
    u32 opaque04;
    struct FldCollisionDescriptor *header; /* 0x08: geometry collision input */
    u32 opaque0C;
    u8 opaque10[0x20];
} FldSpawnCollision;

typedef char FldSpawnCollision_size_must_be_0x30[
    (sizeof(FldSpawnCollision) == 0x30) ? 1 : -1];

#endif
