#include "shared/effect/parManager/decls.h"

/* Allocate one 12-byte descriptor per packet and its attached 16-byte records. */
effTemplatePacketList_t *dds3Particle_SphereSpiral2Create(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x190);
    s32 tailLen = 0x40;
    s32 count;
    s32 perRecord;
    s32 listBytes;
    dds3ParOffsetBuffParts_t *history;
    dds3ParOffsetBuffParts_t *historyBase;
    s32 step;
    u8 *pointBase;
    s32 particleIndex;

    memset(clone, 0, 0x190);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + 0x150, (u8 *)source + source->head.templateSize, tailLen);
    dds3Particle_BasicCreate(clone);
    perRecord = 0;
    if (clone->head.sub.type != 0) {
        count = clone->head.particleCount;
        /* All four supported resource kinds use the same subrecord count. */
        switch (clone->head.sub.type) {
        case PAR_KIND_STATE_HISTORY:
            perRecord = clone->head.sub.templateEntryCount;
            break;
        case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
            perRecord = clone->head.sub.templateEntryCount;
            break;
        case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
            perRecord = clone->head.sub.templateEntryCount;
            break;
        case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
            perRecord = clone->head.sub.templateEntryCount;
            break;
        }
        listBytes = count * 12;
        clone->recordsPerPacket = perRecord;
        clone->listAllocation = sdfMemAllocDirect(listBytes + perRecord * count * 16);
        historyBase = sdfMemLock(clone->listAllocation);
        particleIndex = 0;
        pointBase = (u8 *)historyBase;
        clone->recordList = historyBase;
        pointBase += listBytes;
        if (count > 0) {
            step = perRecord * 16;
            history = historyBase;
            do {
                particleIndex++;
                history->points = (f32 (*)[3])pointBase;
                history->point = 0;
                history->count = 0;
                history++;
                pointBase += step;
            } while (particleIndex < count);
        }
    } else {
        clone->listAllocation = 0;
    }
    dds3Particle_SphereSpiral2AllBuffInit(clone);
    return clone;
}
