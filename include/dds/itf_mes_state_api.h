#ifndef DDS_ITF_MES_STATE_API_H

#define DDS_ITF_MES_STATE_API_H
#include "common.h"

struct BtlFade;

struct dds_instanceMes_tag;

struct ItfMesTextSlots;

/* Entry release may also release the primary speaker-caption text block. */
typedef s32 ItfMesMessageReleaseMode;

enum {
    ITF_MES_RELEASE_ENTRY_ONLY = 0,
    ITF_MES_RELEASE_ENTRY_AND_PRIMARY_TEXT = 1
};

/* Release an interface state's effect resources and clear its effect flags. */
void btlReleaseEffectResourceHandles(struct dds_instanceMes_tag *effect);

#endif /* DDS_ITF_MES_STATE_API_H */
