#include "shared/script/scrScriptProcess/decls.h"

void scrScriptShutdown(dds3ProcessID_t *id)
{
    scrProcessWork_s *pw = dds3GetProcessData(id);

    if (pw != NULL) {
        scrPrintProcedureEnd(pw);
    }
    dds3SetProcessData(id, NULL);
}
