#ifndef DDS_DAT_CALCULATED_STAT_VALUE_API_H

#define DDS_DAT_CALCULATED_STAT_VALUE_API_H
#include "common.h"

#include "dat_stat.h"

typedef s32 DatCalculatedStatValue;

struct dds_datUnitWork_t;

/* Profile results are 1..99; DDS2's slot-adjusted result can also be zero. */
DatCalculatedStatValue datGetClampedProfileAdjustedStat(
    const struct dds_datUnitWork_t *unit, DatBaseStatIndex statIndex);

DatCalculatedStatValue datGetStatWithStatusOverride(
    const struct dds_datUnitWork_t *unit, DatBaseStatIndex statIndex);

#endif
