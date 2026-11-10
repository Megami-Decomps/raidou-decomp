#include "shared/script/scrScriptProcess/decls.h"

void scrSetUserData(dds3ProcessID_t *id, void *p)
{
    scrProcessWork_s *pw = dds3GetProcessData(id);

    pw->data = p;
}
