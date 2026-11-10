#ifndef DDS_DDS3_OBJECT_CONTROL_SLOT_H

#define DDS_DDS3_OBJECT_CONTROL_SLOT_H
#include "common.h"

struct dds3Basic_t;

/* Index domain for object-base control slots. */
typedef s32 Dds3ObjectControlSlotIndex;

enum {
    DDS3_OBJECT_OWNER_SLOT = 0,
    DDS3_OBJECT_DATA_SLOT = 1,
    DDS3_OBJECT_SCRIPT_MODEL_SLOT = 3,
    DDS3_OBJECT_ACTION_B_SLOT = 4,
    DDS3_OBJECT_HANDLER_SLOT = 5
};

#endif /* DDS_DDS3_OBJECT_CONTROL_SLOT_H */
