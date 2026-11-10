#ifndef DDS_BTL_FORMATION_SPACING_CELL_H

#define DDS_BTL_FORMATION_SPACING_CELL_H
#include "common.h"

/* Read-only actor-archive stride used by radial linked formations. */
typedef struct BtlFormationSpacingCell {
    f32 spacing;
    u8 unread04[0x10];
} BtlFormationSpacingCell;

typedef char BtlFormationSpacingCellSizeCheck[
    (sizeof(BtlFormationSpacingCell) == 0x14) ? 1 : -1];

#endif /* DDS_BTL_FORMATION_SPACING_CELL_H */
