#ifndef DDS_MNU_ENTRY_COST_API_H

#define DDS_MNU_ENTRY_COST_API_H
#include "common.h"

struct dds_datUnitWork_t;

typedef s32 MnuEntryCostAffordability;

enum {
    MNU_ENTRY_COST_UNAFFORDABLE = 0,
    MNU_ENTRY_COST_AFFORDABLE = 1
};

/* Compares stored raw HP/MP percentages; equality and other cost kinds pass. */
MnuEntryCostAffordability mnuCanAffordEntryCost(
    u16 commandId, const struct dds_datUnitWork_t *actorEntry);

#endif /* DDS_MNU_ENTRY_COST_API_H */
