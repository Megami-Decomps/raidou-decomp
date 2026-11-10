#include "shared/file/fileManager/decls.h"

void fileSysInit(void) {
    memset(&fileQueueHead, 0, 0x40);
    fileQueueHead.freeCount = 4;
    fileQueueHead.semaId = sdfCreateSema(1, 0x7F, 0);
    fileQueueHead.buffer = EE_ADDR_AS(u8 *, sdfMemLock(sdfMemAllocDirect(0x40000)));
    dds3StartProcess(fileManagerProcName, 0x384, 1, 0, fileManagerProc, 0, 0);
    p2sdSmgBuf = fileManager;
}
