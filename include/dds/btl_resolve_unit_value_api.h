#ifndef DDS_BTL_RESOLVE_UNIT_VALUE_API_H

#define DDS_BTL_RESOLVE_UNIT_VALUE_API_H
#include "dat_effective_affinity_api.h"

#include "btl_unit_value_override_hook.h"

struct BtlUnit;

struct dds_datUnitWork_t;

DatEffectiveAffinityResult btlGetUnitElementAffinity(
    struct BtlUnit *unit, DatAffinitySelector selector);

DatEffectiveAffinityResult btlResolveUnitValueWithOverride(
    struct dds_datUnitWork_t *record, DatAffinitySelector selector);

#endif
