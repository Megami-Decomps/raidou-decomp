#include "shared/effect/parManager/decls.h"

/* Clone the disc prefix/tail plus a separate 16-byte-per-packet auxiliary allocation. */
effTemplatePacketList_t *dds3Particle_SmokeOffsetCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x200);
    sdfMemHandle_t *auxAllocationHandle;
    s32 tailBytes = 0xB0;

    memset(clone, 0, 0x200);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    auxAllocationHandle = sdfMemAllocDirect(clone->head.particleCount << 4);
    clone->auxiliaryAllocation = auxAllocationHandle;
    clone->auxiliaryData = (void *)sdfMemLock(auxAllocationHandle);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SmokeOffsetAllBuffInit(clone);
    return clone;
}
