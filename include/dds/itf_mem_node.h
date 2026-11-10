#ifndef DDS_ITF_MEM_NODE_H

#define DDS_ITF_MEM_NODE_H
#include "common.h"

/* Free-list header stored immediately before each interface-pool payload. */
typedef struct MemNode {
    u32 slotIndex; /* Zero identifies the ring sentinel. */
    struct MemNode *next;
} MemNode;

typedef char MemNodeSizeCheck[(sizeof(MemNode) == 8) ? 1 : -1];

/* Allocation prefix immediately before the returned node ring base. */
struct sdfMemHandle_t;

typedef struct MemRingHeader {
    struct sdfMemHandle_t *allocation;
    MemNode first;
} MemRingHeader;

typedef char MemRingHeaderSizeCheck[(sizeof(MemRingHeader) == 0x0C) ? 1 : -1];

#endif
