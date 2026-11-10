#ifndef DDS_MNU_PARTY_ENTRY_LOOKUP_H

#define DDS_MNU_PARTY_ENTRY_LOOKUP_H
#include "common.h"

struct dds_datUnitWork_t;

/* Match an occupied party slot by table ID without modifying the borrowed entry. */
s32 mnuFindMatchingPartyEntryIndex(const struct dds_datUnitWork_t *borrowedEntry);

/* Return a mask for the first occupied slot matching the borrowed entry's table ID. */
s32 mnuGetMatchingPartyEntryMask(const struct dds_datUnitWork_t *borrowedEntry);

#endif /* DDS_MNU_PARTY_ENTRY_LOOKUP_H */
