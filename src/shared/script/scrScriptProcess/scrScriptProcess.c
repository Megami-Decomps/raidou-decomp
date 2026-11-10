#include "shared/script/scrScriptProcess/decls.h"

s32 scrScriptProcess(dds3ProcessID_t *id)
{
    switch (scrTraceCode(dds3GetProcessData(id))) {
    case SCR_RESULT_ERROR:
        return -1;
    case SCR_RESULT_END:
        return -1;
    case SCR_RESULT_CONTINUE:
    default:
        return 0;
    }
}
