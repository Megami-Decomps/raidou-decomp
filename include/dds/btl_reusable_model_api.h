#ifndef DDS_BTL_REUSABLE_MODEL_API_H

#define DDS_BTL_REUSABLE_MODEL_API_H
#include "common.h"

#include "btl_model_key.h"

/* Available means an active unit can supply matching loaded model state. */
typedef s32 BtlReusableModelAvailability;

enum {
    BTL_REUSABLE_MODEL_UNAVAILABLE = 0,
    BTL_REUSABLE_MODEL_AVAILABLE = 1,
};

BtlReusableModelAvailability btlHasMatchingModel(s32 effectId, BtlModelId modelId);

#endif /* DDS_BTL_REUSABLE_MODEL_API_H */
