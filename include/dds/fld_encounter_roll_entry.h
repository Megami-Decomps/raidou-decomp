#ifndef DDS_FLD_ENCOUNTER_ROLL_ENTRY_H

#define DDS_FLD_ENCOUNTER_ROLL_ENTRY_H
#include "common.h"

typedef s16 FldEncounterEventFlagIndex;

typedef s16 FldEncounterTableIndex;

#define FLD_ENCOUNTER_EVENT_FLAG_INDEX_UNGATED (-1)

#define FLD_ENCOUNTER_TABLE_INDEX_NO_SELECTION (-1)

typedef struct FldEncEntry {
    s16 stage;
    FldEncounterEventFlagIndex eventFlagIndex;
    s16 chancePercent; /* Compared against a random value in [0, 100). */
    FldEncounterTableIndex encounterTableIndex;
} FldEncEntry;

typedef char FldEncEntry_size_must_be_8[
    (sizeof(FldEncEntry) == 8) ? 1 : -1];

#endif
