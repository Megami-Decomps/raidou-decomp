#ifndef DDS_DAT_UNIT_DYING_API_H

#define DDS_DAT_UNIT_DYING_API_H
#include "common.h"

typedef u8 DatUnitDyingResult;

enum {
    DAT_UNIT_DYING_RESULT_NO = 0,
    DAT_UNIT_DYING_RESULT_YES = 1
};

struct dds_datUnitWork_t;

#endif /* DDS_DAT_UNIT_DYING_API_H */
