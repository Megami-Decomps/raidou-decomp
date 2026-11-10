#include "shared/sdk/libkernl/decls.h"

s32 sceTtyInit(void)
{
    ttyPacket_t *packet;

    /* Registration exposes this context to the asynchronous TTY callback.
     * Publish and observe its shared words with native-width accesses. */
    FlushCache(0);
    (*(volatile s32 *)&ttyState.handle) = sceDeci2Open(0x210, &ttyState, sceTtyHandler);
    if ((*(volatile s32 *)&ttyState.handle) < 0) {
        return 0;
    }
    (*(volatile s32 *)&ttyState.sendBusy) = 0;
    (*(volatile s32 *)&ttyState.sendLength) = 0;
    (*(volatile s32 *)&ttyState.receiveLength) = 0;
    ttyState.receiveBuffer = (u8 *)(EE_ADDR(&ttyReceivePacket) | 0x20000000);
    packet = (ttyPacket_t *)(EE_ADDR(&ttySendPacket) | 0x20000000);
    ttyState.sendBuffer = (u8 *)packet;
    packet->protocol = 0x210;
    packet->source = 'E';
    packet->destination = 'H';
    packet->reserved = 0;
    packet->ttyReserved = 0;
    ttyState.queue = gcc2_compiled__10(0x100);
    return 1;
}
