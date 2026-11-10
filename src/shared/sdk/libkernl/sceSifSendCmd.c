#include "shared/sdk/libkernl/decls.h"

s32 sceSifSendCmd(s32 command, void *packet, s32 packetBytes,
                  void *source, void *destination, s32 dataBytes)
{
    return _sceSifSendCmd(command, 0, packet, packetBytes, source, destination, dataBytes);
}
