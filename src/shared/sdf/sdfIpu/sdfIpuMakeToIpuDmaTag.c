#include "shared/sdf/sdfIpu/decls.h"

/* Each 16-byte DMA tag holds control/address bits and a zero reserved half. */
void sdfIpuMakeToIpuDmaTag(sdfIpuDecodeCb_t *cb, u8 *data, s32 leng) {
    s32 remaining = (leng + 15) & ~15;
    struct sdfIpuDmaTag_t *tag;
    addr_t address;
    tag = sdfAllocAuto(((remaining + 0xFFEFF) / 0xFFF00) * 16);
    cb->inputDmaChain = tag;
    address = EE_PHYSICAL(EE_ADDR(data));
    do {
        s32 chunk = remaining > 0xFFF00 ? 0xFFF00 : remaining;
        u32 id;
        remaining -= chunk;
        id = remaining != 0 ? 3 : 0;
        tag->control = ((u16)(chunk >> 4) | (id << 28)) | ((u64)address << 32);
        tag->reserved = 0;
        address += chunk;
        tag++;
    } while (remaining > 0);
    FlushCache(0);
}
