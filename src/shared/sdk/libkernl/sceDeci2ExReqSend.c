#include "shared/sdk/libkernl/decls.h"

s32 sceDeci2ExReqSend(s32 handle, s8 destination)
{
    s32 packet[2];

    packet[0] = handle;
    packet[1] = destination;
    return Deci2Call(-7, packet);
}
