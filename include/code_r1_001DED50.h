#ifndef CODE_R1_001_DED50_H
#define CODE_R1_001_DED50_H

#include "common.h"
#include "sdf_resource.h"

/* Native billboard UVs are four ordered U/V corners, not a float vector. */
typedef struct billTextureQuad_t {
    u16 components[8];
} billTextureQuad_t;

typedef char billTextureQuad_size_must_be_0x10[
    (sizeof(billTextureQuad_t) == 0x10) ? 1 : -1];
/* Producer-built input shared with the generated-texture packet providers.
 * Bytes 0x18..0x1F remain opaque; the provider does not read them. */
typedef struct effGeneratedTextureDescriptor_t {
    f32 position[4];
    f32 scaleX;
    f32 scaleY;
    u8 opaque18[8];
    f32 angle;
    u32 color;
    struct sdfTexHandle_t *texture;
} effGeneratedTextureDescriptor_t;

SIZE_CHECK(effGeneratedTextureDescriptor_size_must_be_0x2C, effGeneratedTextureDescriptor_t, 0x2C);

void effSubmitGeneratedTexturePacket(sdfGrOt_t *surface, effGeneratedTextureDescriptor_t *source);

#endif
