#include "shared/effect/parManager/decls.h"

/* Allocate records followed by their owner tail; clear only two native state words per record. */
dds3Particle_Physics_t *dds3Particle_PhysicsCreate(s32 number) {
    s32 recordBytes = number * sizeof(effectBufferRecord_t);
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(recordBytes + sizeof(dds3Particle_Physics_t));
    effectBufferRecord_t *recordCursor = (void *)sdfMemLock(allocationHandle);
    dds3Particle_Physics_t *bufferTail = (dds3Particle_Physics_t *)((u8 *)recordCursor + recordBytes);

    bufferTail->hMem = allocationHandle;
    bufferTail->pPhysics = recordCursor;
    if (number > 0) {
        s32 remaining = number;
        do {
            remaining--;
            recordCursor->age = 0;
            recordCursor->color = 0;
            recordCursor++;
        } while (remaining != 0);
    }
    return bufferTail;
}
