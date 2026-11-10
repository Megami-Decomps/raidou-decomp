#ifndef DDS_BTL_HUNT_MP_RECOVERY_API_H

#define DDS_BTL_HUNT_MP_RECOVERY_API_H
#include "common.h"

struct BtlUnit;

/* Signed MP amount restored to the acting unit after a hunt. */
typedef s32 BtlHuntMpRecoveryAmount;

/* The target is retained by the paired hunt-recovery interface; this
 * calculation derives the amount from the acting unit's recovery abilities. */
BtlHuntMpRecoveryAmount btlGetHuntMpRecoveryAmount(
    struct BtlUnit *actor, struct BtlUnit *target);

#endif /* DDS_BTL_HUNT_MP_RECOVERY_API_H */
