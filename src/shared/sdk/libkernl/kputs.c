#include "shared/sdk/libkernl/decls.h"

s32 kputs(const char *text)
{
    const char *packet[1];

    packet[0] = text;
    return Deci2Call(16, packet);
}
