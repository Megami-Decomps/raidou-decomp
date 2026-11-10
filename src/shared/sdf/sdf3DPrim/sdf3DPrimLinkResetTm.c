#include "shared/sdf/sdf3DPrim/decls.h"

void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag,
                             s32 (*aAllocator)(s32)) {
    u64 *referencePacket;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    referencePacket = EE_ADDR_AS(u64 *, aAllocator(0x20));
    referencePacket[0] = SDF_DMA_TAG(EE_ADDR(sdf3DPrimIdentityTm), 0x30000008);
    referencePacket[1] = 0x6C07C000ULL << 32;
    QUAD_ZERO(*(u128 *)&referencePacket[2]);
    sdfDrawTagLinkRef(aTag, EE_ADDR(referencePacket));
}
