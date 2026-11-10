#include "shared/sdk/libkernl/decls.h"

s32 _lf_bind(void)
{
    s32 delay;

    if (lfBindStatus < 0) {
        for (;;) {
            if (sceSifBindRpc(&lfRpcClient, 0x80000006, 0) < 0) {
                return -1;
            }
            if (lfRpcClient.server != NULL) {
                lfBindStatus = 0;
                if (sceSifCallRpc(&lfRpcClient, 0xFF, 0, NULL, 0,
                                  &lfRequest, 4, NULL, NULL) < 0) {
                    return -65537;
                }
                memcpy(lfDetectedVersion, &lfRequest, 4);
                return 0;
            }
            for (delay = 0x100000; delay != -1; delay--) {
            }
        }
    }
    return 0;
}
