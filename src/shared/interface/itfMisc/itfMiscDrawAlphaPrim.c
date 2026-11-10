#include "shared/interface/itfMisc/decls.h"

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = EE_ADDR_AS(itfGsState_t *, sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES));

    dst->value = itfAlphaValueTable[tp];
    dst->registerCode = context ? ITF_ALPHA_SECOND_CONTEXT : ITF_ALPHA_FIRST_CONTEXT;
    sdfDrawTagLink(pdrawtag, packet);
}
