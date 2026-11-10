#ifndef DDS_BTL_EFFECT_POSITION_H

#define DDS_BTL_EFFECT_POSITION_H
#include "common.h"

struct BtlUnit;

/* Leaves the selected effect or muzzle position in vf10. */
void btlUnitGetEffectPosVU(struct BtlUnit *unit);

#endif
