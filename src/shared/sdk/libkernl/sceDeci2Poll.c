#include "shared/sdk/libkernl/decls.h"

s32 sceDeci2Poll(s32 handle)
{
    s32 packet[1];

    packet[0] = handle;
    return Deci2Call(4, packet);
}
