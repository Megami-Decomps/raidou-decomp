#include "shared/sdf/sdfIpu/decls.h"

/* Fill at most seven of eight 8-KiB slots. A short non-EOF query waits for more
 * data; EOF permits a partial final read. Copy the final slot's trailing bytes
 * into the prefix mirror, retaining the original copy even for a partial read.
 */
void sdfIpuHandleDmaEnd(sdfIpuDecodeCb_t *feed) {
    s32 writeSlot;
    s32 filledSlots;
    s32 availableBytes;
    addr_t destinationAddress;
    s32 endOfStream;
    filledSlots = feed->filledSlots;
    if (feed->done == 0 && filledSlots < 7) {
        do {
            writeSlot = feed->firstSlot;
            endOfStream = 0;
            destinationAddress = EE_ADDR(feed->scratchBuffer);
            writeSlot += filledSlots;
            if (writeSlot >= 8) {
                writeSlot -= 8;
            }
            destinationAddress += writeSlot << 13;
            destinationAddress = EE_UNCACHED(EE_PHYSICAL(destinationAddress));
            availableBytes = feed->read(feed, feed->strmCallbackParm, SDF_STREAM_READ_QUERY, &endOfStream, 0);
            if (availableBytes < 0x2000) {
                if (endOfStream == 0) {
                    return;
                }
                if (availableBytes > 0) {
                    feed->read(feed, feed->strmCallbackParm, SDF_STREAM_READ_COPY, EE_ADDR_AS(void *, destinationAddress), availableBytes);
                    filledSlots++;
                }
                feed->done = 1;
            } else {
                if (availableBytes == 0x2000) {
                    if (endOfStream != 0) {
                        feed->done = 1;
                    }
                }
                feed->read(feed, feed->strmCallbackParm, SDF_STREAM_READ_COPY, EE_ADDR_AS(void *, destinationAddress), 0x2000);
                filledSlots++;
            }
            if (writeSlot == 7) {
                memcpy((void *)EE_UNCACHED(EE_PHYSICAL(EE_ADDR(feed->scratchBuffer - 0x100))), EE_ADDR_AS(void *, (destinationAddress + 0x1F00)), 0x100);
            }
            feed->filledSlots = filledSlots;
            if (feed->done != 0) {
                break;
            }
        } while (filledSlots < 7);
    }
}
