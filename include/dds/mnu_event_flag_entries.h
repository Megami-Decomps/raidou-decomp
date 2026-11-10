#ifndef DDS_MNU_EVENT_FLAG_ENTRIES_H

#define DDS_MNU_EVENT_FLAG_ENTRIES_H
#include "common.h"

#define MNU_SCENE_FLAG_ENTRY_COUNT 4

#define MNU_PARTY_FLAG_ENTRY_COUNT 16

/* Scene rows carry the two event flags followed by their text-table indices. */
typedef struct MnuSceneFlagEntry {
    s32 requiredFlag;
    s32 handledFlag;
    u16 areaIndex;
    u8 nameIndex;
    u8 unknown0B;
    u16 dialogIndex;
    u16 unknown0E;
} MnuSceneFlagEntry;

/* Each party row maps a party identity to the required/handled flag pair. */
typedef struct MnuPartyFlagEntry {
    s32 requiredFlag;
    s32 handledFlag;
} MnuPartyFlagEntry;

typedef char MnuSceneFlagEntry_size_is_10[(sizeof(MnuSceneFlagEntry) == 0x10) ? 1 : -1];

typedef char MnuPartyFlagEntry_size_is_8[(sizeof(MnuPartyFlagEntry) == 8) ? 1 : -1];

#endif
