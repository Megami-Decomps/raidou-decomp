#include "shared/sdk/libkernl/decls.h"

s32 sceSifResetIop(const char *arguments, s32 mode)
{
    sifDmaTransfer_t transfer;
    u32 destination;
    s32 length;

    sceSifStopDma();
    destination = sceSifGetReg(0x80000000);
    sifResetPacket.mode = mode;
    for (length = 0; arguments[length] != 0; length++) {
        sifResetPacket.arguments[length] = arguments[length];
    }
    sifResetPacket.destination = NULL;
    sifResetPacket.argumentSize = length;
    sifResetPacket.command = 0x80000003;
    sifResetPacket.dataSize = 0;
    sifResetPacket.packetSize = 104;
    transfer.source = EE_ADDR(&sifResetPacket);
    transfer.destination = destination;
    transfer.size = 104;
    transfer.attributes = 0x44;
    sceSifWriteBackDCache(&sifResetPacket, 104);
    sceSifSetReg(4, 0x40000);
    if (sceSifSetDma(&transfer, 1) != 0) {
        sceSifSetReg(4, 0x10000);
        sceSifSetReg(4, 0x20000);
        sceSifSetReg(0x80000002, 0);
        sceSifSetReg(0x80000000, 0);
        return 1;
    }
    return 0;
}
