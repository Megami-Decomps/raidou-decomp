#include "shared/sdf/sdf3DPrim/decls.h"

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32)) {
    u64 *referencePacket;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    sdfMatUpdatePacket(aMat, (s8)sdfFrameId);
    referencePacket = EE_ADDR_AS(u64 *, aAllocator(0x20));
    sdfMatMakePacket(aMat, (struct SdfDmaTagHeader *)referencePacket, (s8)sdfFrameId);
    QUAD_ZERO(*(u128 *)&referencePacket[2]);
    sdfDrawTagLinkRef(aTag, EE_ADDR(referencePacket));
}
