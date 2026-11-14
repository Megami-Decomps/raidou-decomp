#ifndef DDS_FLD_ENCOUNT_FLAGS_H

#define DDS_FLD_ENCOUNT_FLAGS_H
#include "common.h"

/* The player controls the player's object: set as the field hands it the
 * player's control (fldPreparePlayerSceneCameraTarget, a script's
 * PAD_ENABLE), cleared as it takes it back (fldReleaseUnitPad,
 * a script's PAD_DISABLE) and as a scene loads. */

#define FLD_ENCOUNT_FLAG_PLAYER_CONTROL 0x40

#endif /* DDS_FLD_ENCOUNT_FLAGS_H */
