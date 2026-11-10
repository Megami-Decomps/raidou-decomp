#include "shared/sdk/libkernl/decls.h"

s32 sceTtyWrite(const char *buffer, s32 length)
{
    s32 consumed = 0;
    s32 outputLength = 0;
    /* Reuse the live transport status for the request and completion tests. */
    s32 status;
    ttyPacket_t *packet;
    u8 *output;

    if ((*(volatile ttyState_t *)&ttyState).sendBusy != 0) {
        return -1;
    }
    DIntr();
    packet = (ttyPacket_t *)(EE_ADDR(&ttySendPacket) | 0x20000000);
    ttyState.sendBusy = 1;
    ttyState.sendBuffer = (u8 *)packet;
    output = packet->payload;
    /* Native decrement wraps even for negative lengths; only zero stops it. */
    while ((length = (s32)((u32)length - 1)) != -1) {
        if (*buffer == '\n') {
            *output++ = '\r';
            if (++outputLength >= 256) {
                break;
            }
        }
        *output++ = *buffer++;
        outputLength++;
        consumed++;
        if (outputLength >= 256) {
            break;
        }
    }
    {
        volatile ttyState_t *state = &ttyState;

        state->sendLength = outputLength + 12;
        packet->length = state->sendLength;
        status = sceDeci2ReqSend((*(volatile ttyState_t *)&ttyState).handle,
                                 (s8)packet->destination);
        if (status < 0) {
            state->sendBusy = 0;
            EIntr();
            return -1;
        }
        while ((status = state->sendBusy) != 0) {
            sceDeci2Poll((*(volatile ttyState_t *)&ttyState).handle);
        }
        EIntr();
        return consumed;
    }
}
