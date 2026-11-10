#include "shared/sdf/sdfSystem/decls.h"

s32 sdfInitThreadSystem(void) {
    s32 threadId;

    InitThread();
    sdfVblankFlag = 0;
    sdfVblankThreadId = NULL;
    sdfThreadSema = sdfCreateSema(1, 1, 0);
    threadId = CreateThreadSimple((void *)VblankThreadProc, sdfVblankThreadStack, 0x800, 0x3E);
    sdfVblankSema = threadId;
    return _StartThread(threadId, 0);
}
