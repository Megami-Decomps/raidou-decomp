#ifndef CODE_R1_002_C8188_H
#define CODE_R1_002_C8188_H

#include "common.h"
#include "code_r1_001CC1D0.h"
#include "fileManager.h"

/* Submit a single GS ALPHA-register value through the active packet stream. */
void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind);

/* Nonzero keepAllocation transfers the source descriptor to the returned owner. */
struct sprHandle_t *effCreateResourceSlotSetFromAllocation(
    struct sdfMemHandle_t *resourceAllocation, u32 keepAllocation);

void effInitializeSlotWorkFromDescription(struct sprHandle_t *owner, s32 slotIndex,
                                          void *payload);

/* The view owns its descriptions/work and borrows source's texture references; source must outlive it. */
struct sprHandle_t *effCreateResourceSlotSet(struct sprHandle_t *borrowedSource,
                                               u32 slot, u32 count);

void effInitializeSlotWork(struct sprHandle_t *owner, s32 slotIndex);

u32 effDestroyResourceSlotSet(struct sprHandle_t *owner);

struct effMappedResource_t;

u32 effConfigureIndexedSlotMaterial(struct sprHandle_t *work, s32 index,
                                    struct effMappedResource_t *data, s32 item,
                                    u32 materialFlags, u32 materialValue, u32 stateFlags);

/* Loads base/name; the finished job stores the resource in *outInstance. */
fileRequest_t *effRequestResourceByMode(const char *base, const char *name, u32 mode, void **outInstance);

void effRequestMappedResource(const char *pFileName, const char *pBundle, effMappedResource_t **outMappedResource);

#endif
