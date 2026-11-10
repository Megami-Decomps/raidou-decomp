#include "shared/sdf/sdfIpu/decls.h"

s32 sdfIpuToDmacHandler(void) {
    sdfIpuDecodeCb_t *stream = sdfIpuPendingCb;
    if (stream != NULL) {
        sdfIpuDoneFrame = stream->frame;
        sdfIpuPendingCb = NULL;
        if (stream->inputFeedEnabled != 0 && stream->active == 1) {
            if (stream->inputFeedDmaInFlight != 0) {
                stream->inputFeedDmaInFlight = 0;
                stream->firstSlot++;
                if (stream->firstSlot == 8) {
                    stream->firstSlot = 0;
                }
                stream->filledSlots--;
            }
            sdfIpuHandleDmaEnd(stream);
            sdfIpuSetToIpuDma(stream);
        }
    }
    EE_SYNC_EI();
    return 0;
}
