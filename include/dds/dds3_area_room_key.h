#ifndef DDS_DDS3_AREA_ROOM_KEY_H

#define DDS_DDS3_AREA_ROOM_KEY_H
#include "common.h"

struct dds3Basic_t;

/* Signed packed area/room key; -1 means the world object has no location yet. */
typedef s32 Dds3AreaRoomKey;

#define DDS3_AREA_ROOM_KEY_UNASSIGNED ((Dds3AreaRoomKey)-1)

#endif /* DDS_DDS3_AREA_ROOM_KEY_H */
