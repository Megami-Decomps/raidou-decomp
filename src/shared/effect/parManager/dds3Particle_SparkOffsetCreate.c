#include "shared/effect/parManager/decls.h"

/* Clone the offset-gravity prefix/tail, allocate records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SparkOffsetCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x190);
    s32 tailBytes = 0x40;

    memset(clone, 0, 0x190);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SparkOffsetAllBuffInit(clone);
    return clone;
}
