#ifndef DDS_EFF_OBJECT_NODE_H

#define DDS_EFF_OBJECT_NODE_H
#include "common.h"

/* Intrusive model-object list node; the object payload stays opaque here. */
typedef struct EffectObjectNode {
    void *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

typedef char EffectObjectNode_size_must_be_0xC[
    (sizeof(EffectObjectNode) == 0xC) ? 1 : -1];

#endif /* DDS_EFF_OBJECT_NODE_H */
