#include "shared/sdf/sdfPrint/decls.h"

void sdfPrintRpcThread(void)
{
    sifRpcDataQueue_t queue;
    sifPrintRpcServerData_t server;

    sceSifSetRpcQueue(&queue, GetThreadId());
    sceSifRegisterRpc((struct sifRpcServerData_t *)&server, 0x32647270, (sceSifRpcFunc)sdfPrintRpcFunc, sdfPrintRpcBuff, 0, NULL, (struct sifRpcDataQueue_t *)&queue);
    sceSifRpcLoop(&queue);
}
