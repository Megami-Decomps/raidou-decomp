#include "shared/script/scrScriptProcess/decls.h"

dds3ProcessID_t *scrStartScript2(s32 priority, void *scrdata, s32 startproc)
{
    return scrStartScriptProcess(priority, fldFlowParse(scrdata, startproc));
}
