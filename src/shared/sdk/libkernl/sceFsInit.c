#include "shared/sdk/libkernl/decls.h"

s32 sceFsInit(void)
{
    fsPowerOffState_t *powerOff = &fsPowerOffState;
    s32 previousInterrupts;
    s32 delay;
    fsRequestEntry_t *entry;

    sceSifInitRpc(0);
    powerOff->callback = NULL;
    powerOff->argument = NULL;
    previousInterrupts = DIntr();
    sceSifAddCmdHandler(0x80000011, _sceFs_Rcv_Intr, fsCmdHandlerContext);
    sceSifAddCmdHandler(0x80000013, (void (*)(void *, void *))_sceFs_Poff_Intr, powerOff);
    if (previousInterrupts)
        EIntr();
    for (;;) {
        if (sceSifBindRpc(&fsRpcClient, 0x80000001, 0) < 0)
            return -1;
        if (fsRpcClient.server != NULL)
            break;
        for (delay = 0x100000; delay != -1; --delay) {
        }
    }
    gcc2_compiled__13();
    WaitSema(fsIobSema);
    for (entry = fsIobTable; entry < &fsIobTable[32]; entry++)
        entry->active = 0;
    SignalSema(fsIobSema);
    fsRegistration.receiveBuffer = fsCompletionBuffer;
    if (sceSifCallRpc(&fsRpcClient, 255, 0, &fsRegistration, 4,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0)
        return -0x10001;
    *(fsVersionTag_t *)fsDetectedVersion =
        *(const fsVersionTag_t *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    fsInitialized = 1;
    return 0;
}
