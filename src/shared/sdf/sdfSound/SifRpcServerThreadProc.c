#include "shared/sdf/sdfSound/decls.h"

void SifRpcServerThreadProc(void)
{
    sifRpcDataQueue_t queue;
    sifRpcServerData_t server;
    s32 sema = sdfCreateSema(0, 1, 0);

    sifRpcServerSema = sema;
    if (sema <= 0) {
        for (;;) {
        }
    }
    sceSifSetRpcQueue(&queue, GetThreadId());
    sceSifRegisterRpc((struct sifRpcServerData_t *)&server, 0x54524E53, (sceSifRpcFunc)sdfFileRpcHandleCmd, sdfFileRpcQueue, 0, NULL, (struct sifRpcDataQueue_t *)&queue);
    sceSifRpcLoop(&queue);
}
