#include "shared/sdk/libkernl/decls.h"

void sceSifRpcLoop(sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *request;

    for (;;) {
        while ((request = sceSifGetNextRequest(queue)) != NULL) {
            sceSifExecRequest(request);
        }
        SleepThread();
    }
}
