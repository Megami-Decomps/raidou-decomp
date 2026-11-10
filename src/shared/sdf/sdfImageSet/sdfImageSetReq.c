#include "shared/sdf/sdfImageSet/decls.h"

void sdfImageSetReq(sdfImageSetReqParam_t *parm)
{
    sdfImageSetCb_t *cb = &sdfImageSetCb;
    sdfImageSetPacket_tag *release;
    sdfImageSetTag_t *tagTail;
    u64 *setup;
    u64 *packet;
    void *buf;
    u32 pixelFormat;
    u16 w;
    u16 h;
    u16 bufferWidth;
    s32 qwc;
    s32 blocks;
    s32 remaining;
    s32 widthUnits;
    s32 last;
    addr_t src;
    u64 formatAndAddr;
    u64 coords;
    u64 size;

    pixelFormat = parm->pixelFormat;
    w = parm->w;
    h = parm->h;
    bufferWidth = parm->bufferWidth;
    widthUnits = bufferWidth >> 6;
    if (widthUnits == 0) {
        widthUnits = 1;
    }

    qwc = sdfImageDataSize(pixelFormat, w, h);
    blocks = (qwc + 0x7FEF) / 0x7FF0;
    buf = sdfChipAllocDirect(blocks * 0x30 + 0x80);
    release = (sdfImageSetPacket_tag *)buf;
    release->next = NULL;
    release->chip = parm->data;
    release->memHandle = parm->memHandle;
    release->freeMode = parm->freeMode;

    setup = (u64 *)((u8 *)buf + 0x10);
    packet = (u64 *)((u8 *)buf + 0x70);
    formatAndAddr = ((u64)pixelFormat << 56) | ((u64)widthUnits << 48) | ((u64)(parm->addr >> 6) << 32);
    coords = ((u64)parm->y << 48) | ((u64)parm->x << 32);
    size = ((u64)h << 32) | w;

    setup[0] = 0x0000000010000005ULL;
    setup[1] = 0x5000000500000000ULL;
    setup[2] = 0x1000000000000004ULL;
    setup[3] = 0x000000000000000EULL;
    setup[4] = formatAndAddr;
    setup[5] = 0x50ULL;
    setup[6] = coords;
    setup[7] = 0x51ULL;
    setup[8] = size;
    setup[9] = 0x52ULL;
    setup[10] = 0;
    setup[11] = 0x53ULL;

    src = EE_PHYSICAL(EE_ADDR(parm->data));
    remaining = qwc;
    last = 0;
    do {
        s32 chunk = 0x7FF0;

        if (remaining < 0x7FF1) {
            chunk = remaining;
            last = 1;
        }

        packet[0] = 0x0000000010000001ULL;
        packet[1] = 0x5000000100000000ULL;
        packet[2] = ((u64)chunk | ((u64)last << 15)) | 0x0800000000000000ULL;
        packet[4] = ((u32)chunk & 0xFFFF) | 0x30000000ULL | ((u64)src << 32);
        packet[5] = ((u64)((u32)chunk | 0x51000000)) << 32;

        src += (u32)chunk << 4;
        remaining -= chunk;
        packet += 6;
    } while (last == 0);

    WaitSema(cb->semaId);

    if (cb->packetTail != NULL) {
        cb->packetTail->next = release;
    } else {
        cb->packetHead = release;
    }
    cb->packetTail = release;

    setup = (u64 *)EE_PHYSICAL(EE_ADDR(setup));
    tagTail = cb->tagTail;
    if (tagTail != NULL) {
        tagTail->vifCodes = 0;
        tagTail->tag = ((u64)EE_ADDR(setup) << 32) | 0x20000000ULL;
    } else {
        cb->tagHead = EE_ADDR(setup);
    }
    cb->tagTail = (sdfImageSetTag_t *)packet;
    SignalSema(cb->semaId);
}
