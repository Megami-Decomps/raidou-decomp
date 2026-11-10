#ifndef DDS_DAT_AFFINITY_SELECTOR_H

#define DDS_DAT_AFFINITY_SELECTOR_H
#include "dat_affinity_selector_type.h"

/* Map one exact channel flag to an affinity selector. */
DatAffinitySelector datFlagToElementIndex(s32 channelFlag);

#endif /* DDS_DAT_AFFINITY_SELECTOR_H */
