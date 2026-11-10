#include "shared/sdf/sdfImageSet/decls.h"

void sdfImageSetTrans(void)
{
    sdfImageSetCb_t *cb = &sdfImageSetCb;
    sdfImageSetPacket_tag *release;
    sdfImageSetTag_t *tagTail;
    u32 dmaAddr;

    WaitSema(cb->semaId);
    release = cb->packetHead;
    dmaAddr = cb->tagHead;
    tagTail = cb->tagTail;
    sdfImageSetClearCb(cb);
    SignalSema(cb->semaId);

    if (dmaAddr != 0) {
        s32 threadId = GetThreadId();
        s32 handlerId = AddDmacHandler2(1, (s32 (*)(s32, void *, void *))sdfImageSetDmacHandler, 0, EE_ADDR_AS(void *, threadId));
        u64 tag = SDF_DMA_TAG(EE_ADDR(D_005293A0), 0x20000000);
        u32 *chan;

        tagTail->vifCodes = 0;
        tagTail->tag = tag;
        chan = sceDmaGetChan(1);
        HW_WRITE(chan, HW_READ(chan) | 0x40);
        FlushCache(0);
        sceDmaSend(chan, EE_ADDR_AS(void *, dmaAddr));
        SleepThread();
        RemoveDmacHandler(1, handlerId);
    }

    while (release != NULL) {
        sdfImageSetPacket_tag *next = release->next;

        switch (release->freeMode) {
        case SDF_IMAGESET_MEM_HANDLE:
            sdfMemFreeDirect(release->memHandle);
            break;
        case SDF_IMAGESET_MEM_CHIP:
            sdfChipFreeDirect(release->chip);
            break;
        }
        sdfChipFreeDirect(release);
        release = next;
    }
}
