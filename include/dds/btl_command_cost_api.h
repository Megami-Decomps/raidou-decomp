#ifndef DDS_BTL_COMMAND_COST_API_H

#define DDS_BTL_COMMAND_COST_API_H
#include "btl_command_id.h"

struct BtlUnit;

struct dds_datUnitWork_t;

/* Ability-scaled HP/MP command amount; distinct from affordability results. */
typedef s32 BtlCommandVitalCostAmount;

/* These APIs take the command-table key, not the computed cost. */
BtlCommandVitalCostAmount btlApplyCommandAbilityMultiplier(
    struct dds_datUnitWork_t *battler, BtlCommandId commandId);

BtlCommandVitalCostAmount btlGetCombinedPartyCommandPower(
    struct dds_datUnitWork_t *base, struct BtlUnit *first,
    struct BtlUnit *second, struct BtlUnit *third, BtlCommandId commandId);

/* Both slot-cost helpers convert command IDs to affinity-table rows. */
s8 btlGetLinkageSlotCost(BtlCommandId commandId);

s32 btlGetSlotValueAdjustedForSpecialAbility(
    struct BtlUnit *battler, BtlCommandId commandId);

#endif /* DDS_BTL_COMMAND_COST_API_H */
