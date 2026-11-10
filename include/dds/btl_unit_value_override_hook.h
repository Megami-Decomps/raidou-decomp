#ifndef DDS_BTL_UNIT_VALUE_OVERRIDE_HOOK_H

#define DDS_BTL_UNIT_VALUE_OVERRIDE_HOOK_H
#include "dat_affinity_selector_type.h"

struct dds_datUnitWork_t;

/* Return -1 to use the normal affinity-table resolver. */
typedef s32 (*BtlUnitValueOverrideHook)(
    struct dds_datUnitWork_t *record, DatAffinitySelector selector);

#endif /* DDS_BTL_UNIT_VALUE_OVERRIDE_HOOK_H */
