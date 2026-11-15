#ifndef CODE_R1_002_C8188_H
#define CODE_R1_002_C8188_H

#include "common.h"
#include "code_r1_001CC1D0.h"
#include "fileManager.h"

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

struct EffMappedResource;

u32 effConfigureIndexedSlotMaterial(struct EffectSlotSet *work, s32 index,
                                    struct EffMappedResource *data, s32 item,
                                    u32 materialFlags, u32 materialValue, u32 stateFlags);

/* Loads base/name; the finished job stores the resource in *outInstance. */
fileRequest_t *effRequestResourceByMode(const char *base, const char *name, u32 mode, void **outInstance);

void effRequestMappedResource(const char *pFileName, const char *pBundle, EffMappedResource **outMappedResource);

#endif
