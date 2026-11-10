#include "shared/sdf/sdfTmx/decls.h"

void *sdfFileRpcHandler(s32 function, void *request, s32 requestSize) {
    sdfFileRpcCb_t *state = &sdfFileRpcState;
    sdfFileCmdHandle_t *deviceState;
    u32 command = (u32)function;

    switch (command) {
    case SDF_STREAM_COMMAND_CREATE_STATE:
        state->deviceState = sdfFileCmdReadOpen((const char *)request, sdfFileRpcReadCallback, 0);
        if (state->deviceState != 0) {
            WaitSema(state->semaphore);
            deviceState = state->deviceState;
            if (deviceState->errorCode != 0) {
                sdfFileCmdClearError(deviceState);
                sdfFileCmdFree(state->deviceState);
                state->deviceState = 0;
                deviceState = 0;
            }
        } else {
            deviceState = 0;
        }
        sdfFileRpcBuf[0] = EE_ADDR_S(deviceState);
        break;
    case SDF_STREAM_COMMAND_CONTROL:
        sdfFileCmdFileSize(state->deviceState, EE_ADDR_S(request), EE_ADDR_S(request));
        WaitSema(state->semaphore);
        sdfFileRpcBuf[0] = state->readResult;
        break;
    case SDF_STREAM_COMMAND_READ:
        state->remainingBytes = ((sdfFileRpcReadReq_t *)request)->byteCount;
        state->transferredBytes = 0;
        state->destination = EE_ADDR_AS(u8 *, ((sdfFileRpcReadReq_t *)request)->destinationAddress);
        sdfFileRpcContinueRead();
        WaitSema(state->semaphore);
        sdfFileRpcBuf[0] = state->transferredBytes;
        break;
    case SDF_STREAM_COMMAND_RELEASE_STATE:
        sdfFileCmdClose(state->deviceState);
        WaitSema(state->semaphore);
        sdfFileCmdFree(state->deviceState);
        state->deviceState = 0;
        sdfFileRpcBuf[0] = 0;
        break;
    }
    return sdfFileRpcBuf;
}
