#ifndef DDS_DAT_CALCULATE_COMMAND_BASE_VALUE_API_H

#define DDS_DAT_CALCULATE_COMMAND_BASE_VALUE_API_H
#include "btl_command_id.h"

struct dds_datUnitWork_t;

typedef u32 DatCommandBaseCost;

/* commandId must select a command record; the provider indexes it unchecked. */
/* Returns the unscaled HP/MP cost. Zero can mean free, unsupported, or gated. */
DatCommandBaseCost datCalculateCommandBaseValue(
    struct dds_datUnitWork_t *battler, BtlCommandId commandId);

#endif /* DDS_DAT_CALCULATE_COMMAND_BASE_VALUE_API_H */
