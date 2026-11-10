#ifndef DDS_FLD_AREA_ENCOUNTER_TABLE_H

#define DDS_FLD_AREA_ENCOUNTER_TABLE_H
#include "common.h"

/* One area's 64 room choices and their flag-selected encounter alternatives. */
typedef struct FieldAreaEncounterRow {
    s32 defaultEncounter; /* 0x00: fallback encounter ID */
    s16 lowerPriorityFlagId;      /* 0x04 */
    s16 lowerPriorityEncounterId; /* 0x06: selected by lowerPriorityFlagId */
    s16 higherPriorityFlagId;     /* 0x08: tested before the lower-priority flag */
    s16 higherPriorityEncounterId; /* 0x0A: selected by higherPriorityFlagId */
} FieldAreaEncounterRow;

typedef struct FieldAreaEncounterTable {
    s32 area;                     /* 0x000: area matched against scene state */
    FieldAreaEncounterRow rooms[64]; /* 0x004: per-room encounter choices */
} FieldAreaEncounterTable;

typedef char FieldAreaEncounterRow_size_check[
    (sizeof(FieldAreaEncounterRow) == 0x0C) ? 1 : -1];

typedef char FieldAreaEncounterTable_size_check[
    (sizeof(FieldAreaEncounterTable) == 0x304) ? 1 : -1];

#endif /* DDS_FLD_AREA_ENCOUNTER_TABLE_H */
