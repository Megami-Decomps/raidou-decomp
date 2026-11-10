#ifndef DDS_EFF_LIGHT_H

#define DDS_EFF_LIGHT_H
#include "dds3obj.h"

struct sdf3DLightParallelPacket_t;

/* One directional source is two adjacent vec4s in the lighting SDK API. */
typedef union EffLightSourceVectors {
    f32 vectors[2][4];
    struct {
        f32 color[4];
        f32 direction[4];
    } named;
} EffLightSourceVectors;

typedef char EffLightSourceVectors_size_must_be_0x20[
    (sizeof(EffLightSourceVectors) == 0x20) ? 1 : -1];

/* The first flag enables publishing this light's source and ambient vectors;
 * the second blends those vectors over the configured frame count. */
enum EffLightFlags {
    EFF_LIGHT_FLAG_UPDATE_COLOR_VECTORS = 1,
    EFF_LIGHT_FLAG_BLEND_COLOR_VECTORS = 2
};

/* Complete 0x7C-byte payload owned by kind-9 world nodes. */
typedef struct EffLightData {
    EffLightSourceVectors sourceVectors;
    u8 pad20[0x20];
    f32 ambientColor[4];
    f32 parameter50;
    f32 parameter54;
    f32 parameter58;
    u32 word5C;
    u32 mode;
    u32 flags;
    u32 packedColorA;
    u32 packedColorB;
    u16 blendFrames;
    u16 unk72;
    dds_dds3ObjectBaseControlWorkData_t *baseControlWorkData; /* Owned block released by lightdestructor. */
    struct sdf3DLightParallelPacket_t *buffer;
} EffLightData;

typedef char EffLightData_size_must_be_0x7C[(sizeof(EffLightData) == 0x7C) ? 1 : -1];

#endif
