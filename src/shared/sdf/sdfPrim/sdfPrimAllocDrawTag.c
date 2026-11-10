#include "shared/sdf/sdfPrim/decls.h"

sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*aAllocator)(s32)) {
    sdfDrawTag_t *list;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    list = (sdfDrawTag_t *)EE_ADDR_PTR(aAllocator(SDF_PACKET_LIST_BYTES));
    sdfDrawTagInit(list);
    return list;
}
