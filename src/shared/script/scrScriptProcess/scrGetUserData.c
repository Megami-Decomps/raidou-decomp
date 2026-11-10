#include "shared/script/scrScriptProcess/decls.h"

void *scrGetUserData(dds3ProcessID_t *id)
{
    scrProcessWork_s *pw = dds3GetProcessData(id);

    return pw->data;
}
