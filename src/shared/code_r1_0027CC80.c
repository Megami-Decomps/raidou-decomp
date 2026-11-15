#include "common.h"
#include "sdk_libc.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/btl.h"


#include "dds/ee_mmi.h"

#define BTL_AI_WEIGHT_MASK 0xFFFF

#define BTL_HEALTH_RATE_SCALE 100

#define BTL_PARTY_QUERY_MASK 0x221

#define BTL_PARTY_ACTIVE_FLAGS 0x201

#define BTL_ENEMY_QUERY_MASK 0x421

#define BTL_ENEMY_ACTIVE_MASK 0x401

#define BTL_ENEMY_ACTIVE_FLAGS 0x401

#define BTL_REQUIRED_ACTION_CATEGORY_COUNT 10

#define BTL_ENTRY_CODE_SCAN_COUNT 10

#define BTL_AI_COUNTER_SATURATION 0x100

#define BTL_AI_COUNTER_MAX 0xFF

#define BTL_LOW_HP_ACTION_DELAY 0xF

#define BTL_LOW_HP_PERCENT_LIMIT 0x1E

#define BTL_LOW_HP_BUCKET_LIMIT 0x1E

#define BTL_LOW_HP_ENEMY_COUNT_LIMIT 2

extern u32 GetRand(u32);
extern u32 btlPreviousAiCandidateBucket;
extern void (*btlAiActionHandlers[])(BtlTask *, u32, s32);
extern s32 GetPercentage(void);

/* Sum of two 0..0xFFF rolls, folded into 0..0xFFF; /41 turns it into one of 100 buckets. */
static inline u32 btlRollTwice(void) {
    u32 value = GetRand(0x1000);
    value += GetRand(0x1000);
    return value & 0xFFF;
}

s32 GetPercentage(void);

/* Out parameters of the action-id lookup. */
typedef struct BtlActionLookup {
    u16 slot;
    s32 result;
} BtlActionLookup;

/* Rolls a bucket; if it lands within 3 of the previous one, rolls again. */
s32 GetPercentage(void) {
    u32 slot = btlRollTwice() / 0x29;
    if (btlPreviousAiCandidateBucket >= slot - 3 && btlPreviousAiCandidateBucket <= slot + 3) {
        u32 reroll = btlRollTwice();
        btlPreviousAiCandidateBucket = slot;
        return reroll / 0x29;
    }
    btlPreviousAiCandidateBucket = slot;
    return slot;
}
INCLUDE_SDATA(const s32, "code_r1_0027CC80", GetPercentageData);

INCLUDE_SDATA(const s32, "code_r1_0027CC80", sbtlData94);

INCLUDE_SDATA(const s32, "code_r1_0027CC80", g_itemNo);

INCLUDE_SDATA(const s32, "code_r1_0027CC80", sbtlData95);

INCLUDE_SDATA(const s32, "code_r1_0027CC80", cmpSetupData4);

