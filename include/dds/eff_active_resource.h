#ifndef DDS_EFF_ACTIVE_RESOURCE_H

#define DDS_EFF_ACTIVE_RESOURCE_H
#include "common.h"

/* Per-instance header shared by every resource-kind callback. payload points
 * to kind-specific bytes immediately after this 0x40-byte allocation prefix;
 * resource is the separately owned kind-specific object. */
typedef struct EffActiveResource {
    f32 position[4];
    f32 orientation[4];
    f32 scale;
    u32 color;
    u32 frame;
    union {
        u32 index;
        s32 signedIndex;
        u16 shortIndex;
    } kind;
    void *resource;
    u8 pad_34[4];
    u8 *payload;
    u8 pad_3C[4];
} EffActiveResource;

typedef char EffActiveResource_size_must_be_0x40[
    (sizeof(EffActiveResource) == 0x40) ? 1 : -1];

#endif /* DDS_EFF_ACTIVE_RESOURCE_H */
