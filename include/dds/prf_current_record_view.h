#ifndef DDS_PRF_CURRENT_RECORD_VIEW_H

#define DDS_PRF_CURRENT_RECORD_VIEW_H
struct dds_datUnitWork_t;

struct DatProfileRecord;

/* Borrow the selected record from live game state as a read-only view. */
const struct DatProfileRecord *ptyGetCurrentProfileRecord(struct dds_datUnitWork_t *unit);

#endif
