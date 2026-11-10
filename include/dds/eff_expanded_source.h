#ifndef DDS_EFF_EXPANDED_SOURCE_H

#define DDS_EFF_EXPANDED_SOURCE_H
#include "eff_expanded_list.h"

/* Serialized texture-animation list header, count entry records, then payloads.
 * flags is copied to EffExpandedList and tested by animation sampling. */
typedef struct EffExpandedSource {
    u8 pad00[4];
    u32 count;
    u32 flags;
    u8 pad0C[4];
    EffExpandedEntry entries[1];
} EffExpandedSource;

typedef char EffExpandedSource_size_check[
    sizeof(EffExpandedSource) == 0x20 ? 1 : -1];

#endif
