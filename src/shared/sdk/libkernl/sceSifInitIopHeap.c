#include "shared/sdk/libkernl/decls.h"

s32 sceSifInitIopHeap(void)
{
    s32 delay;

    for (;;) {
        if (sceSifBindRpc((rpcClient *)iopHeapRpcClient, 0x80000003, 0) < 0) {
            return -1;
        }
        if (((rpcClient *)iopHeapRpcClient)->server != NULL) {
            break;
        }
        for (delay = 0x100000; delay != -1; delay--) {
        }
    }
    {
        /* SDK 2.9 otherwise separates the flag-address and return phases.
         * Keep this real address/store/result lifetime in the return register;
         * the binding is a reconstruction aid, not original-source evidence.
         * This initialized lifetime starts after the final call.
         */
        FAKEMATCH_ASM_REG(saddr_t, result, "v0") = EE_ADDR_S(&iopHeapStatus);
        *EE_ADDR_AS(s32 *, result) = 0;
        result = 0;
        return result;
    }
}
