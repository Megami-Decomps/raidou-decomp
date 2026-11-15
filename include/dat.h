#ifndef DAT_H
#define DAT_H

#include "common.h"

/* One 0x20C-byte encounter adjustment record. Sound selection shares the
 * header with condition/variant data; four complete weighted groups follow. */
typedef struct BattleAdjustmentEntry {
    u16 battleMode;
    u16 weight;
    s8 value;
    u8 serialEncounterChanceAdjustment; /* 0x05: input percentage before world-flag scaling and clamp. */
} BattleAdjustmentEntry;

typedef struct BattleAdjustmentGroup {
    s32 interval;
    BattleAdjustmentEntry entries[20];
} BattleAdjustmentGroup;

typedef struct BattleAdjustmentRecord {
    u16 encounterParamA; /* 0x00: copied to BtlState.encounterParamA by 001A11F0. */
    u16 encounterParamB; /* 0x02 */
    u16 streamSelection; /* 0x04: default stream selector used by 001F3278. */
    u8 pad06[2];
    u32 conditions[3]; /* Packed kind and solar-phase-mask/model-flag condition. */
    u8 variantCodes[8];
    BattleAdjustmentGroup groups[4];
} BattleAdjustmentRecord;

typedef char BattleAdjustmentEntrySizeCheck[sizeof(BattleAdjustmentEntry) == 6 ? 1 : -1];

typedef char BattleAdjustmentGroupSizeCheck[sizeof(BattleAdjustmentGroup) == 0x7C ? 1 : -1];

typedef char BattleAdjustmentRecordSizeCheck[sizeof(BattleAdjustmentRecord) == 0x20C ? 1 : -1];

extern BattleAdjustmentRecord *datBattleAdjustmentRecords;

#include "enums.h"

struct sdatUnitWork_t;

void CallProcEnd_cmpStatus(void);
void UserInit(void);
s32 datAddDevil(void);
char *datGetPlayerExp(CMP_TITLE_ID index);
void datGlobalWorkDestroy(void);
void datGlobalWorkInit(void);
s32 sdatCalc_GetBadStatus(struct sdatUnitWork_t *unit);

#endif
