#ifndef DDS_EVT_ITEM_VALUE_THRESHOLD_API_H

#define DDS_EVT_ITEM_VALUE_THRESHOLD_API_H
#include "pty_item_id.h"

typedef u8 EvtItemValueThresholdResult;

enum {
    EVT_ITEM_VALUE_THRESHOLD_NOT_MET = 0,
    EVT_ITEM_VALUE_THRESHOLD_MET = 1
};

/* Flag items test their model flag and ignore minimumQuantity; others compare count against it. */
EvtItemValueThresholdResult evtCheckValueThreshold(PtyItemId itemId, s32 minimumQuantity);

#endif /* DDS_EVT_ITEM_VALUE_THRESHOLD_API_H */
