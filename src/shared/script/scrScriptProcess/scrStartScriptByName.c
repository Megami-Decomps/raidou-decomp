#include "shared/script/scrScriptProcess/decls.h"

void scrStartScriptByName(s32 priority, char *name, s32 messageHandle)
{
    scrStartScriptProcess(priority, scrLoadScriptFile(name, messageHandle));
}
