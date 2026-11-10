#ifndef DDS_ACTIVE_SOUND_NODE_H

#define DDS_ACTIVE_SOUND_NODE_H
#include "common.h"

/* Per-unit active sound-list entry, allocated at exactly 0x14 bytes. */
typedef struct ActiveSoundNode {
    u32 flags; /* 0x00: bits 1/8 are inspected by the resource-lifetime checks. */
    u8 loadState; /* 0x04: sound-file task sets this to 2 while loading. */
    u8 pad05[3];  /* 0x05: no accesses established. */
    u32 packedTrackPosition; /* 0x08: key passed to packed-track load queries. */
    struct ActiveSoundNode *previous; /* 0x0C */
    struct ActiveSoundNode *next; /* 0x10 */
} ActiveSoundNode;

typedef char ActiveSoundNode_size_must_be_0x14[
    (sizeof(ActiveSoundNode) == 0x14) ? 1 : -1];

#endif /* DDS_ACTIVE_SOUND_NODE_H */
