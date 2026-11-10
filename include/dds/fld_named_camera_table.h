#ifndef DDS_FLD_NAMED_CAMERA_TABLE_H

#define DDS_FLD_NAMED_CAMERA_TABLE_H
#include "fld_camera_follow_row.h"

/* Each title's FLDALL.TBL loader reads 0x500 bytes into this 32-row bank.
 * The linked-object camera consumes each 0x28-byte row through the named
 * node strings, signed axis selector, and two floating-point bounds. */
enum { FLD_NAMED_CAMERA_COUNT = 32 };

typedef char FldNamedCameraTableSizeCheck[
    (sizeof(FldNamedCamera[FLD_NAMED_CAMERA_COUNT]) == 0x500) ? 1 : -1];

#if defined(VERSION_DDS1)

extern FldNamedCamera D_0032F510[FLD_NAMED_CAMERA_COUNT];

#elif defined(VERSION_DDS2)

extern FldNamedCamera D_0038A9B0[FLD_NAMED_CAMERA_COUNT];

#endif

#endif /* DDS_FLD_NAMED_CAMERA_TABLE_H */
