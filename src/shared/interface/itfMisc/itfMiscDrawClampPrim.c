#include "shared/interface/itfMisc/decls.h"

/* Write CLAMP for the selected GS context. */
void itfMiscDrawClampPrim(sdfDrawTag_t *pdrawtag, s32 context, s32 wms, s32 wmt, s32 min_u, s32 max_u, s32 min_v, s32 max_v) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = EE_ADDR_AS(itfGsState_t *, sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES));

    dst->value = ((u64)max_v << 34) | ((u64)min_v << 24) | (max_u << 14) | (min_u << 4) | (wmt << 2) | wms;
    dst->registerCode = context ? 9 : 8;
    sdfDrawTagLink(pdrawtag, packet);
}
