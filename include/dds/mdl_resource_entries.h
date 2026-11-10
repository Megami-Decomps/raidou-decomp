#ifndef DDS_MDL_RESOURCE_ENTRIES_H

#define DDS_MDL_RESOURCE_ENTRIES_H
#include "mdl.h"

/* Key for a part-info record list; -1 selects the context-wide list. */
typedef s32 MdlPartInfoListKey;

enum {
    MDL_PART_INFO_LIST_CONTEXT_WIDE = -1
};

/* Effect-item owner slot; -1 tags context-wide items outside per-slot replacement. */
typedef s32 MdlMotionSlotIndex;

enum {
    MDL_MOTION_SLOT_CONTEXT_WIDE = -1
};

#endif /* DDS_MDL_RESOURCE_ENTRIES_H */
