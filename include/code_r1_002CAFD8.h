#ifndef CODE_R1_002_CAFD8_H
#define CODE_R1_002_CAFD8_H

#include "common.h"
#include "code_r1_001CC1D0.h"

void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z,
                                 s32 drawFlags, struct EffectSlotSet *object,
                                 s32 index, s32 surfaceIndex);

/* Known continuation bit; other EffectSlotDescription.flags meanings remain unspecified. */
enum EffectSlotDescriptionFlag {
    EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION = 0x20,
};

#endif
