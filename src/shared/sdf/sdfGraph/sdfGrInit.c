#include "shared/sdf/sdfGraph/decls.h"

void sdfGrInit(s32 aPacketBuffSize) {
    s32 thread;
    sdfGrFrameSet_t *graph = &sdfGrFrameSet;

    sceGsResetPath();
    sdfGrScreenMode = 1;
    sdfGrSetFrameSetMode(graph, 1, 1);
    sdfGrMakeFrameSet(graph);
    sdfGrGsResetDone = 0;
    sdfDispFrameId[0] = sdfDispFrameId[1] = -1;
    /* Plain: runs before the draw thread exists, so no waiter. */
    sdfGrDrawBusy = -1;
    reqGsReset = 1;
    sdfGrMakePacketBuff(aPacketBuffSize);
    sdfGrInitDraw();
    sdfTexInit();
    dds3GsSema = sdfCreateSema(1, 0x20, 0);
    dds3GsThreadId = sdfCreateSema(0, 0x20, 0);
    thread = CreateThreadSimple((void *)sdfDrawThread, sdfDrawThreadStack, 0x8000, 0x44);
    sdfDrawThreadId = thread;
    _StartThread(thread, 0);
    sdfStartThreadAndWait(&sdfDispThreadCB, sdfDispThread, sdfDispThreadStack,
                          0x1000, 0x40, 0);
}
