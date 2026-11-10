#ifndef DDS_DAT_EFFECTIVE_AFFINITY_API_H

#define DDS_DAT_EFFECTIVE_AFFINITY_API_H
#include "dat_effective_affinity_result_type.h"

#include "dat_affinity_selector.h"

struct dds_datUnitWork_t;

DatEffectiveAffinityResult datGetEffectiveAffinity(
    const struct dds_datUnitWork_t *unit, DatAffinitySelector selector);

#endif /* DDS_DAT_EFFECTIVE_AFFINITY_API_H */
