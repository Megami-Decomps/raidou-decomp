#include "shared/sdk/libkernl/decls.h"

s32 iRotateThreadReadyQueue(s32 priority)
{
    u32 index;

    if ((u32)priority >= 0x80 || topThreadId == 0) {
        return -1;
    }

    index = threadCommandQueue.writeIndex & 0x1FF;
    threadCommandQueue.writeIndex = index + 1;
    threadCommandQueue.commands[index][0] = 1;
    threadCommandQueue.commands[index][1] = (u8)priority;
    iSignalSema(threadMgrSema);
    return priority;
}
