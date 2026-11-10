#ifndef DDS_FILE_SLOT_FLAGS_CHANGE_API_H

#define DDS_FILE_SLOT_FLAGS_CHANGE_API_H
#include "common.h"

/* Compare the current slot-flag mirror with the previous value. After a load,
 * unchanged accepts the loaded flags; changed restores the prior flag snapshot. */
typedef s32 FileSlotFlagsChangeResult;

enum {
    FILE_SLOT_FLAGS_UNCHANGED = 0,
    FILE_SLOT_FLAGS_CHANGED = 1
};

FileSlotFlagsChangeResult fileLoadStateChanged(void);

#endif /* DDS_FILE_SLOT_FLAGS_CHANGE_API_H */
