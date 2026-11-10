#include "shared/sdk/libkernl/decls.h"

s32 sceDeci2Open(u16 protocol, void *argument, deci2Handler_t handler)
{
    deci2Registration_t packet;

    packet.protocol = protocol;
    packet.argument = argument;
    packet.handler = handler;
    packet.storage = (void *)(EE_ADDR(deci2PacketStorage) | 0x20000000);
    return Deci2Call(1, &packet);
}
