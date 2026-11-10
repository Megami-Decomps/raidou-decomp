#include "shared/sdf/sdfImageSet/decls.h"

void sdfImageSetChainReleaseMem(saddr_t p)
{
    sdfImageSetCb_t *cb = &sdfImageSetCb;
    sdfImageSetPacket_tag *release;

    if (p != 0) {
        release = sdfChipAllocZeroDirect(0xA0);
        if (sdfChipCheck(EE_ADDR_AS(void *, p)) != 0) {
            release->chip = EE_ADDR_AS(void *, p);
            release->freeMode = SDF_IMAGESET_MEM_CHIP;
        } else {
            release->freeMode = SDF_IMAGESET_MEM_HANDLE;
            release->memHandle = sdfMemGetHandleByPtr(EE_ADDR_AS(void *, p));
        }
        WaitSema(cb->semaId);
        if (cb->packetTail != NULL) {
            cb->packetTail->next = release;
        } else {
            cb->packetHead = release;
        }
        cb->packetTail = release;
        SignalSema(cb->semaId);
    }
}
