#ifndef DDS_EFF_MODEL_OWNER_H

#define DDS_EFF_MODEL_OWNER_H
#include "common.h"

struct dds3ModelHandle_t;

struct sdf3DLightParallelPacket_t;

/* Heap-owned model wrapper allocated as a 0x10-byte chip block. */
typedef struct EffModelOwner {
    f32 scale;
    struct dds3ModelHandle_t *model;
    struct sdf3DLightParallelPacket_t *ownedBuffer;
    u32 flags;
} EffModelOwner;

/* Floor-list lifecycle and deferred-refresh state shared by both providers. */
enum {
    EFF_MODEL_OWNER_FLAG_UPDATE_PENDING = 0x01,
    EFF_MODEL_OWNER_FLAG_DESTROY_REQUESTED = 0x02,
    EFF_MODEL_OWNER_FLAG_FLOOR_LISTED = 0x04,
    EFF_MODEL_OWNER_FLAG_DESTROY_READY = 0x08,
    EFF_MODEL_OWNER_FLAG_LISTED_OBJECT = 0x10,
    EFF_MODEL_OWNER_FLAG_UPDATE_DEFERRED = 0x20
};

typedef char EffModelOwner_size_check[
    (sizeof(EffModelOwner) == 0x10) ? 1 : -1];

#endif /* DDS_EFF_MODEL_OWNER_H */
