#ifndef DDS_ITF_PANEL_API_H

#define DDS_ITF_PANEL_API_H
#include "common.h"

#include "sdf_packet_list.h"

struct dds_itfpanel_t;

struct sdfTexHandle_t;

/* The panel owns its allocations and borrows the caller's shared texture. */
struct dds_itfpanel_t *itfCreateUiSpriteByKind(s32 kind,
    const struct sdfTexHandle_t *borrowedTexture);

#endif /* DDS_ITF_PANEL_API_H */
