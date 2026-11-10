#ifndef DDS_EFF_RESOURCE_OWNER_H

#define DDS_EFF_RESOURCE_OWNER_H
#include "common.h"

#include "file.h"

#include "mdl.h"

#include "sdf.h"

#include "eff.h"

/* An effect resource owner: the lifetime fields shared by both games, the
 * animation header copied from the resource at +8 (DDS1's shorter payload is
 * EffectModelHeaderCopy's 0x3C bytes), and the retained queue array. */

#ifdef VERSION_DDS1

typedef struct EffResourceOwner {
    u32 count;
    u32 currentFrame;
    SdfColorTrack colorTrack; /* 0x08 */
    SdfAlphaTrack alphaTrack; /* 0x2C */
    u32 plainEntry; // 0x3C: add the model entry plain instead of flagged
    f32 frameStep; /* 0x40 */
    struct FileQueue **entries; /* 0x44: retained queue-pointer array */
    struct sdfMemHandle_t *queueAllocation; /* 0x48: queue-pointer array descriptor */
    dds3ModelHandle_t *model;
} EffResourceOwner;

typedef char EffResourceOwner_size_check[(sizeof(EffResourceOwner) == 0x50) ? 1 : -1];

#else

typedef struct EffResourceConfig {
    SdfColorTrack colorTrack; /* 0x00 */
    SdfAlphaTrack alphaTrack; /* 0x24 */
    s32 plainEntry; /* Duration, also selects plain versus flagged entry. */
    f32 frameStep;
} EffResourceConfig;

/* Option-1 resource payload: copied as 0x74 bytes by effCreateResourceOwner.
 * Requests and signed indices are consumed by effResolveRequestPositionIntoVu. */
typedef struct EffResourceExtendedConfig {
    EffResourceConfig base;
    u8 enableExplicitEndpoints; /* 0x3C */
    u8 unknown3D[3];
    EffectVectorRequest start; /* 0x40 */
    EffectVectorRequest end; /* 0x48 */
    s16 startIndex; /* 0x50 */
    s16 endIndex; /* 0x52 */
    u8 preserveBaseScale; /* 0x54 */
    u8 unknown55[0x1F];
} EffResourceExtendedConfig;

typedef struct EffResourceOwner {
    u32 count;
    s32 currentFrame;
    EffResourceConfig base;
    EffResourceExtendedConfig extended;
    FileQueue **entries;
    sdfMemHandle_t *buffer;
    dds3ModelHandle_t *model;
} EffResourceOwner;

typedef char EffResourceConfig_size_check[sizeof(EffResourceConfig) == 0x3C ? 1 : -1];

typedef char EffResourceExtendedConfig_size_check[sizeof(EffResourceExtendedConfig) == 0x74 ? 1 : -1];

typedef char EffResourceOwner_size_check[sizeof(EffResourceOwner) == 0xC4 ? 1 : -1];

#endif

#endif /* DDS_EFF_RESOURCE_OWNER_H */
