#include "shared/interface/itfMesManager/decls.h"

void ResolveResourceAddress(itfMesResource_t *pdata)
{
    s32 *payload = (s32 *)pdata->entries;

    if (pdata->relocated == 0) {
        sdfAddressResolver(payload, EE_ADDR_S(payload), (u8 *)pdata + pdata->fixupTableOffset, pdata->fixupTableBytes);
        pdata->relocated = 1;
    }
}
