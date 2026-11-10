#include "shared/sdf/sdfIpu/decls.h"

s32 sdfIpuClose(void) {
    if (sdfFromIpuDmacHandlerHandle != 0) {
        sdfIpuRemoveHandler(EE_ADDR_AS(void *, sdfFromIpuDmacHandlerHandle));
        sdfFromIpuDmacHandlerHandle = 0;
    }
    DisableDmac(3);
    if (sdfToIpuDmacHandlerHandle != 0) {
        sdfIpuRemoveHandler(EE_ADDR_AS(void *, sdfToIpuDmacHandlerHandle));
        sdfToIpuDmacHandlerHandle = 0;
    }
    return DisableDmac(4);
}
