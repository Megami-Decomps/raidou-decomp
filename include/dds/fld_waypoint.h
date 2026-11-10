#ifndef DDS_FLD_WAYPOINT_H

#define DDS_FLD_WAYPOINT_H
#include "../fldWap.h"
#include "fld.h"

/* The loader/copy providers transfer the complete WAP payload: 0x6CA0 in
 * DDS1 and 0x6D00 in DDS2. Their actor tables begin at +0xA0/+0x100,
 * after five/eight 0x20-byte headers, and contain 256 0x6C-byte rows. */

#ifdef VERSION_DDS1

#else

#endif

typedef struct dds_fldWaypointBlock_t {
    fldWaypointHeader_t headers[FLD_WAYPOINT_HEADER_COUNT];
    dds_doormovetbl_s actors[256];
} dds_fldWaypointBlock_t;

typedef char FldWaypointActorSizeCheck[(sizeof(dds_doormovetbl_s) == 0x6C) ? 1 : -1];

#endif
