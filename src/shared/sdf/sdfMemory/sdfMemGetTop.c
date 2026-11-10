#include "shared/sdf/sdfMemory/decls.h"

sdfMemHandle_t *sdfMemGetTop(void)
{
    return sdfMemCB.headGuard.next;
}
