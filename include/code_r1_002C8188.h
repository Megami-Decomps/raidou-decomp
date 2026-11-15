#ifndef CODE_R1_002_C8188_H
#define CODE_R1_002_C8188_H

#include "common.h"
#include "code_r1_001CC1D0.h"

/* Submit a single GS ALPHA-register value through the active packet stream. */
void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind);

/* Nonzero keepAllocation transfers the source descriptor to the returned owner. */
struct EffectSlotSet *effCreateResourceSlotSetFromAllocation(
    struct sdfMemHandle_t *resourceAllocation, u32 keepAllocation);

void effInitializeSlotWorkFromDescription(struct EffectSlotSet *owner, s32 slotIndex,
                                          void *payload);

/* The view owns its descriptions/work and borrows source's texture references; source must outlive it. */
struct EffectSlotSet *effCreateResourceSlotSet(struct EffectSlotSet *borrowedSource,
                                               u32 slot, u32 count);

void effInitializeSlotWork(struct EffectSlotSet *owner, s32 slotIndex);

u32 effDestroyResourceSlotSet(struct EffectSlotSet *owner);

#endif
