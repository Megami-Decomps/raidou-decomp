#ifndef DDS_FLD_STOP_ENTRY_H

#define DDS_FLD_STOP_ENTRY_H
#include "common.h"

typedef struct FldStopEntry {
    u32 unk0;
    const u8 *name;
} FldStopEntry;

typedef char FldStopEntry_size_must_be_8[
    (sizeof(FldStopEntry) == 8) ? 1 : -1];

#endif
