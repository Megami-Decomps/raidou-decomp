#include "shared/sdk/libkernl/decls.h"

s32 sceDeci2ExSend(s32 handle, void *buffer, u16 length)
{
    deci2Transfer_t packet;

    packet.handle = handle;
    packet.buffer = buffer;
    packet.length = length;
    return Deci2Call(-6, &packet);
}
