#ifndef DDS_SDF_TEXTURE_CLAMP_H

#define DDS_SDF_TEXTURE_CLAMP_H
#include "common.h"

#if !defined(SMT3_DEFINES_SdfTextureClampMode)
/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;
#endif

enum {
    SDF_TEXTURE_CLAMP_MODE_DEFAULT = 0xFF,
    SDF_TEXTURE_CLAMP_STATE_MASK = 0x0F
};

struct sdfTexHandle_t;

#endif /* DDS_SDF_TEXTURE_CLAMP_H */
