#ifndef DDS_EVT_ACTIVE_ENTRY_FLAG_API_H

#define DDS_EVT_ACTIVE_ENTRY_FLAG_API_H
#include "common.h"

typedef s32 EvtActiveEntryFlagState;

enum EvtActiveEntryFlagStateValue {
    EVT_ACTIVE_ENTRY_FLAG_CLEAR = 0,
    EVT_ACTIVE_ENTRY_FLAG_SET = 1
};

EvtActiveEntryFlagState evtIsActiveFlagSet(s32 flagIndex);

#endif /* DDS_EVT_ACTIVE_ENTRY_FLAG_API_H */
