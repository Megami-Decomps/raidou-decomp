#ifndef DDS_ITF_MES_ENTRY_START_API_H

#define DDS_ITF_MES_ENTRY_START_API_H
#include "common.h"

#include "itf_mes_entry_index.h"

#include "itf_mes_window_index.h"

/* Outcome reported by starting an entry in a message window. */
typedef s32 ItfMesEntryStartResult;

enum {
    ITF_MES_ENTRY_START_EMPTY_TABLE = 0,
    ITF_MES_ENTRY_START_STARTED = 1
};

ItfMesEntryStartResult itfMesStartEntry(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex);

#endif /* DDS_ITF_MES_ENTRY_START_API_H */
