#ifndef DDS_BTL_SPECIAL_ABILITY_H

#define DDS_BTL_SPECIAL_ABILITY_H
#include "common.h"

#include "dat_skill_id.h"

struct dds_datUnitWork_t;

typedef s32 BtlSpecialAbilityAvailability;

enum {
    BTL_SPECIAL_ABILITY_UNAVAILABLE = 0,
    BTL_SPECIAL_ABILITY_AVAILABLE = 1,
};

/* Returns availability after ownership and any ability-specific phase checks. */
BtlSpecialAbilityAvailability btlCheckSpecialAbility(
    const struct dds_datUnitWork_t *record, DatSkillId skillId);

#endif /* DDS_BTL_SPECIAL_ABILITY_H */
