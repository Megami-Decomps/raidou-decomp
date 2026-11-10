#include "shared/sdf/sdfSound/decls.h"

u32 p2sdSetPcmCallback(addr_t aCallback)
{
    if (p2sdPcmBusy != 0) {
        return 0;
    }
    p2sdPcmCallback = aCallback;
    return aCallback;
}
