#ifndef ITF_PANEL_H
#define ITF_PANEL_H

#include "common.h"
#include "sdf_resource.h"

/* Common draw record allocated by the interface object constructors (0x40). */
/* Allocation fields own general-heap descriptors, not retained payload addresses. */
typedef struct dds_itfpanel_t {
    sdfMemHandle_t *allocation;
    sdfMemHandle_t *payloadAllocation;
    u32 *payload;
    s32 renderValue;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 textRightX;
    s32 screenY;
    s32 scrollSpan; /* Panel edge-fade divisor; both games read it as a signed word. */
    /* RGBA words consumed by the panel packet renderers. */
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    u8 kind; /* Unsigned index into the panel handler tables. */
    u8 pad3D[3];
} dds_itfpanel_t;

#endif
