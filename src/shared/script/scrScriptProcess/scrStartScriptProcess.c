#include "shared/script/scrScriptProcess/decls.h"

dds3ProcessID_t *scrStartScriptProcess(s32 priority, scrProcessWork_s *pw)
{
    return pw->ProcessID = dds3StartProcess((char *)pw->ProcTable[pw->StartProc].Name, priority, 1, 1,
                                            scrScriptProcess, scrScriptShutdown, pw);
}
