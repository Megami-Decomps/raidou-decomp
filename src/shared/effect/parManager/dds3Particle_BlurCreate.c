#include "shared/effect/parManager/decls.h"

/* Allocate contiguous point slabs, slot records and the trailing table header. */
dds3Particle_BlurBuff_t *dds3Particle_BlurCreate(u32 number, u16 length, billHandle_t **pBill_Handle) {
    u32 pointBytes = length * sizeof(u128);
    u32 slabBytes = pointBytes + sizeof(dds3Particle_BlurBuffList_t);
    u32 totalBytes = slabBytes * number;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(totalBytes + sizeof(dds3Particle_BlurBuff_t));
    u8 *base = (u8 *)sdfMemLock(allocation);
    dds3Particle_BlurBuff_t *table = (dds3Particle_BlurBuff_t *)(base + totalBytes);
    u128 *points = (u128 *)base;
    dds3Particle_BlurBuffList_t *slotBase = (dds3Particle_BlurBuffList_t *)(points + length * number);
    dds3Particle_BlurBuffList_t *slots;
    u32 index = 0;

    table->point = length;
    table->pBill_Handle = pBill_Handle;
    table->hMem = allocation;
    table->number = number;
    table->blurList = slotBase;
    if (number != 0) {
        slots = table->blurList;
        do {
            slots[index].pPos = points;
            slots[index].count = 0;
            slots[index].abgr = 0;
            points += length;
            index++;
        } while (index < number);
    }
    return table;
}
