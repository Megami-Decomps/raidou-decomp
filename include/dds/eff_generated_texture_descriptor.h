#ifndef DDS_EFF_GENERATED_TEXTURE_DESCRIPTOR_H

#define DDS_EFF_GENERATED_TEXTURE_DESCRIPTOR_H
#include "common.h"

struct sdfTexHandle_t;

/* Producer-built input shared with the generated-texture packet providers.
 * Bytes 0x18..0x1F remain opaque; the provider does not read them. */
typedef struct EffGeneratedTextureDescriptor {
    f32 position[4];
    f32 scaleX;
    f32 scaleY;
    u8 opaque18[8];
    f32 angle;
    u32 color;
    struct sdfTexHandle_t *texture;
} EffGeneratedTextureDescriptor;

typedef char EffGeneratedTextureDescriptorSizeAssert[
    (sizeof(EffGeneratedTextureDescriptor) == 0x2C) ? 1 : -1];

#endif
