#include "shared/sdk/libkernl/decls.h"

s32 _sceSifSendCmd(s32 command, s32 interruptMode, void *packet, s32 packetBytes,
                    void *source, void *destination, s32 dataBytes)
{
    sifDmaTransfer_t transfers[2];
    sifCommandHeader_t *header = packet;
    s32 count;

    if ((u32)packetBytes - 16U > 96U) {
        return 0;
    }
    count = 0;
    if (dataBytes > 0) {
        transfers[count].source = EE_ADDR(source);
        transfers[count].destination = EE_ADDR(destination);
        transfers[count].size = dataBytes;
        transfers[count].attributes = 0;
        header->dataSize = dataBytes;
        header->destination = EE_ADDR(destination);
        count++;
        if (interruptMode & 4) {
            sceSifWriteBackDCache(source, dataBytes);
        }
    } else {
        header->dataSize = 0;
        header->destination = 0;
    }
    transfers[count].source = EE_ADDR(packet);
    transfers[count].destination = EE_ADDR(sifCommandContext.remoteAddress);
    transfers[count].size = packetBytes;
    header->command = command;
    header->packetSize = packetBytes;
    transfers[count].attributes = 0x44;
    sceSifWriteBackDCache(packet, packetBytes);
    count++;
    if (interruptMode & 1) {
        return isceSifSetDma(transfers, count);
    }
    return sceSifSetDma(transfers, count);
}
