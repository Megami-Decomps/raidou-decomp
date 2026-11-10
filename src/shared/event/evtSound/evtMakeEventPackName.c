#include "shared/event/evtSound/decls.h"

void evtMakeEventPackName(s32 index, char *buffer)
{
    sprintf(buffer, evtPackNameFormat, index);
}
