#ifndef DDS_BTL_MODEL_KEY_H

#define DDS_BTL_MODEL_KEY_H
#include "common.h"

/* Selects the human/devil model resource namespace; zero selects human data. */
typedef s32 BtlModelResourceKind;

/* Model number paired with a resource kind in the battle cache key. */
typedef s32 BtlModelId;

#endif /* DDS_BTL_MODEL_KEY_H */
