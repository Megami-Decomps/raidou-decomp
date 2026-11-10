#ifndef DDS_BTL_ACTOR_GROUP_AVERAGE_API_H

#define DDS_BTL_ACTOR_GROUP_AVERAGE_API_H
#include "common.h"

/* Selects actors by matching bits in their copied dds_datUnitWork_t flags word. */
typedef u32 BtlPartyRecordFlagsMask;

s32 btlAverageAllCurrentForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);

s32 btlAverageAllMaximumForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);

s32 btlAverageAllLevelsForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);

#endif /* DDS_BTL_ACTOR_GROUP_AVERAGE_API_H */
