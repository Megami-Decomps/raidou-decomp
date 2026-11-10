#include "shared/sdf/sdfMemory/decls.h"

/* Never decrements zero. */
void sdfMemUnlock(sdfMemHandle_t *aHandle)
{
    u16 mlock = aHandle->mlock;

    if (mlock != 0) {
        aHandle->mlock = mlock - 1;
    }
}
