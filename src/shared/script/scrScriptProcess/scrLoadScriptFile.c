#include "shared/script/scrScriptProcess/decls.h"

scrProcessWork_s *scrLoadScriptFile(char *name, s32 messageHandle)
{
    sdfMemHandle_t *mem;
    void *data;
    scrProcessWork_s *pw;

    mem = sdfDevLoadFileEx(name, (u32 *)&data, NULL);
    if (data == NULL) {
        return NULL;
    }

    pw = fldFlowParse(data, messageHandle);
    if (pw != NULL) {
        pw->MemHandle = mem;
    }
    return pw;
}
