#include "shared/sdk/libkernl/decls.h"

void _InitSys(void)
{
    supplement_crt0();
    GetSystemCallTableEntry();
    InitAlarm();
    InitThread();
    InitExecPS2();
    InitTLBFunctions();
}
