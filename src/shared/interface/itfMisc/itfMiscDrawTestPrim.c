#include "shared/interface/itfMisc/decls.h"

/* Write TEST for the selected GS context, not the ALPHA blend register. */
void itfMiscDrawTestPrim(sdfDrawTag_t *pdrawtag, u64 para, s32 context) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = EE_ADDR_AS(itfGsState_t *, sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES));

    dst->value = para;
    dst->registerCode = context ? ITF_TEST_SECOND_CONTEXT : ITF_TEST_FIRST_CONTEXT;
    sdfDrawTagLink(pdrawtag, packet);
}
