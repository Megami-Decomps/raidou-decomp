#include "shared/sdk/libkernl/decls.h"

s32 InitThread(void)
{
    kernelThreadParam_t thread;
    rpcSemaParam sema;

    if (topThreadId > 0) {
        return -1;
    }
    sema.maxCount = 255;
    sema.initCount = 0;
    threadMgrSema = CreateSema(&sema);
    if (threadMgrSema < 0) {
        return -1;
    }
    thread.entry = topThread;
    thread.stack = topThreadStack;
    thread.stackSize = 0x400;
    thread.gp = _gp;
    thread.priority = 0;
    topThreadId = CreateThread(&thread);
    if (topThreadId < 0) {
        DeleteSema(threadMgrSema);
        return -1;
    }
    threadCommandQueue.readIndex = 0;
    threadCommandQueue.writeIndex = 0;
    _StartThread(topThreadId, &threadCommandQueue);
    ChangeThreadPriority(GetThreadId(), 1);
    return topThreadId;
}
