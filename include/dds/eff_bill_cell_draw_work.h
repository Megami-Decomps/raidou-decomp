#ifndef DDS_EFF_BILL_CELL_DRAW_WORK_H

#define DDS_EFF_BILL_CELL_DRAW_WORK_H
#include "common.h"

typedef struct BillCellDrawWork {
    u8 opaque00[0x10];
    u8 transform[0x10];
    f32 scale;
    s32 baseColor;
    u32 frameLimit;
    u8 opaque2C[4];
    u32 *instances;
    u8 *config;
} BillCellDrawWork;

typedef char BillCellDrawWorkSizeCheck[(sizeof(BillCellDrawWork) == 0x38) ? 1 : -1];

#endif
