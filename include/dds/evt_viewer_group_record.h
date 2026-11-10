#ifndef DDS_EVT_VIEWER_GROUP_RECORD_H

#define DDS_EVT_VIEWER_GROUP_RECORD_H
#include "common.h"

typedef struct EvtViewerGroupRecord {
    u8 opaque00[8];
    s32 group;       /* 0x08 */
    s32 type;        /* 0x0C */
    u8 opaque10[0x10];
} EvtViewerGroupRecord;

typedef char EvtViewerGroupRecord_size_must_be_0x20[
    (sizeof(EvtViewerGroupRecord) == 0x20) ? 1 : -1];

#endif
