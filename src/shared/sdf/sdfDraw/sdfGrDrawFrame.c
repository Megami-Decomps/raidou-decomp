#include "shared/sdf/sdfDraw/decls.h"

void sdfGrDrawFrame(void)
{
    sdfDrawReqRec_t *request;
    sdfDrawTag_t *drawTag;
    sdfPreDrawHook_t *hook;
    sceDmaChan *channel;
    sdfStoreTransfer_t *transfer;
    u32 mask, resumeAddress, address;
    u8 bufferSlot, frameId, firstFlag, drawCount;
    s32 remaining, count;

    for (;;) {
        request = &sdfGrDrawState[sdfDrawReqQueueIndex];
        drawTag = request->drawTag;
        if (drawTag == NULL) {
            SignalSema(dds3GsThreadId);
            break;
        }
        if (request->drawCount == 2) {
            SignalSema(dds3GsThreadId);
            break;
        }
        if (request->drawCount == 0) {
            bufferSlot = sdfFrameEvenOdd ^ 1;
            request->bufferSlotIndex = bufferSlot;
            sdfGrBufferSlot = bufferSlot;
        } else {
            bufferSlot = request->bufferSlotIndex ^ 1;
            request->bufferSlotIndex = bufferSlot;
            sdfGrBufferSlot = bufferSlot;
        }
        request->bufferIndex = sdfGrDrawFrameId;
        sdfDeliverDrawStart(request->bufferIndex, request->queuedBufferIndex);
        SignalSema(dds3GsThreadId);
        if (sdfDrawImageTrans != 0) {
            sdfDrawImageTrans = 0;
            sdfImageSetTrans();
        }
        for (hook = request->preDraw; hook != NULL; hook = hook->next) {
            hook->predrawFunc(hook, request->bufferIndex);
        }
        FlushCache(0);
        channel = sceDmaGetChan(1);
        HW_FIELD_OR(channel->chcr, 0xC0);
        sdfDispThreadFlag = 3;
        sceDmaSend(channel, EE_ADDR_AS(void *, EE_PHYSICAL(drawTag->head)));
        sdfGrDrawFrameTagHead = drawTag->head;
        for (;;) {
            SleepThread();
            if (sdfDrawFrameFlag != 0) return;
            if (((HW_FIELD_READ(channel->chcr) >> 16) & 0x7000) == 0x7000) {
                break;
            }
            mask = (u32)sceGsGetIMR();
            sceGsPutIMR(mask | 0x200);
            HW_WRITE(GS_CSR, 2);
            HW_FIELD_OR(channel->chcr, 0x100);
            SleepThread();
            if ((HW_READ(GS_CSR) & 2) == 0) {
                do {
                    if (sdfDrawFrameFlag != 0) {
                        sceGsPutIMR(mask);
                        return;
                    }
                } while ((HW_READ(GS_CSR) & 2) == 0);
            }
            HW_WRITE(GS_CSR, 0);
            sceGsPutIMR(mask);
            HW_FIELD_OR(channel->chcr, 0x100);
            SleepThread();
            resumeAddress = HW_FIELD_READ(channel->tadr);
            transfer = EE_ADDR_AS(sdfStoreTransfer_t *, HW_FIELD_READ(channel->madr));
            while ((HW_READ(VIF1_STAT) & 3) != 0) {
                if (sdfDrawFrameFlag != 0) return;
            }
            while ((HW_READ(GIF_STAT) & 0x80) != 0) {
                if (sdfDrawFrameFlag != 0) return;
            }
            HW_WRITE(GIF_CTRL, 8);
            HW_WRITE(VIF1_STAT, 0x800000);
            HW_WRITE(GS_BUSDIR, 1);
            HW_WRITE(GIF_CTRL, 0);
            remaining = transfer->length;
            address = transfer->address;
            while (remaining > 0) {
                count = remaining;
                if (count > 0xF000) {
                    count = 0xF000;
                }
                remaining -= count;
                sceDmaRecvN(channel, EE_ADDR_AS(void *, address), count);
                SleepThread();
                if (sdfDrawFrameFlag != 0) break;
                address += count * 16;
            }
            HW_WRITE(VIF1_STAT, 0);
            HW_WRITE(GS_BUSDIR, 0);
            HW_FIELD_AND(channel->chcr, ~1);
            if (sdfDrawFrameFlag != 0) return;
            if (transfer->callback != NULL) transfer->callback(transfer->callbackParameter);
            sceDmaSend(channel, EE_ADDR_AS(void *, resumeAddress));
        }
        while ((HW_READ(VIF1_STAT) & 3) != 0) {
        }
        while ((HW_READ(GIF_STAT) & 0x180) != 0) {
        }
        sdfDeliverDrawEnd(request->drawCount, request->bufferIndex, request->bufferSlotIndex);
        drawCount = request->drawCount;
        frameId = sdfGrDrawFrameId;
        sdfGrDrawFirstFlag = 1;
        request->drawCount = drawCount + 1;
        sdfGrDrawFrameId = frameId ^ 1;
        break;
    }
    firstFlag = sdfGrDrawFirstFlag;
    sdfDrawPublishFlag = 1;
    if (firstFlag != 0) sdfDevConsDrawAll();
    sdfDelayedResrcProc();
}
