#include "shared/sdf/sdfIpu/decls.h"

void sdfIpuInit(void) {
    s32 thread;
    sceIpuInit();
    HW_WRITE(IPU_CMD, 0x90000000);
    sdfIpuTaskLink = 0;
    sdfCurrIpuDecodeCb = NULL;
    sdfFromIpuDmacHandlerHandle = DmacAddHandler(1, 3, sdfIpuFromDmacHandler, -1, 0);
    EnableDmac(3);
    sdfToIpuDmacHandlerHandle = DmacAddHandler(1, 4, sdfIpuToDmacHandler, -1, 0);
    EnableDmac(4);
    thread = CreateThreadSimple((void *)sdfIpuDecodeThread, sdfIpuDecodeThStack, 0x800, 0x46);
    sdfIpuDecodeThId = thread;
    _StartThread(thread, 0);
}
